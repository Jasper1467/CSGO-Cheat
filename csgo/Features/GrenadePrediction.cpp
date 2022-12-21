#include "GrenadePrediction.h"

#include "../SDK/CEntity.h"
#include "../Utilities/Game.h"
#include "../Utilities/Client.h"
#include "../Utilities/Math.h"
#include "../Menu/Config/Vars.h"
#include "../Utilities/Render.h"

void Features::GrenadePrediction::Predict()
{
	// TODO: Make these values dynamic
	constexpr float restitution = 0.45f;
	constexpr float power[ ] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{}, grenade_act{ 1 };
	Vector pos, thrown_direction, start, eye_origin;
	QAngle angles, thrown;

	//	first time setup
	static auto sv_gravity = Interfaces::m_pCVar->FindVar( "sv_gravity" );

	//	calculate step and actual gravity value
	gravity = sv_gravity->GetFloat( ) / 8.0f;
	step = Interfaces::m_pGlobals->m_flIntervalPerTick;

	//	get local view and eye origin
	eye_origin = Client::m_pLocal->GetEyePos();
	angles = Client::m_pCmd->m_angViewAngles;

	//	copy current angles and normalise pitch
	thrown = angles;

	if( thrown.pitch < 0 ) {
		thrown.pitch = -10 + thrown.pitch * ( ( 90 - 10 ) / 90.0f );
	}
	else {
		thrown.pitch = -10 + thrown.pitch * ( ( 90 + 10 ) / 90.0f );
	}

	//	find out how we're throwing the grenade
	auto primary_attack = Client::m_pCmd->m_nButtons & IN_ATTACK;
	auto secondary_attack = Client::m_pCmd->m_nButtons & IN_ATTACK2;

	if( primary_attack && secondary_attack ) {
		grenade_act = ACT_LOB;
	}
	else if( secondary_attack ) {
		grenade_act = ACT_DROP;
	}

	//	apply 'magic' and modulate by velocity
	unk01 = power[ grenade_act ];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	//	here's where the fun begins
	Math::AngleVectors( thrown, thrown_direction );

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = ( thrown_direction * new_velocity ) + Client::m_pLocal->m_vecVelocity();

	//	let's go ahead and predict
	for( auto time = 0.0f; index < 500; time += step ) {
		pos = start + thrown_direction * step;

		//	setup trace
		trace_t trace;
		CTraceFilter filter(Client::m_pLocal);

		Interfaces::m_pTrace->TraceRay( Ray_t{ start, pos }, MASK_SOLID, &filter, &trace );

		//	modify path if we have hit something
		if( trace.flFraction != 1.0f ) {
			thrown_direction = trace.plane.vecNormal * -2.0f * thrown_direction.Dot( trace.plane.vecNormal ) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += ( step * ( 1.0f - trace.flFraction ) );
		}

		//	check for detonation
		auto detonate = Detonated( Client::m_pLocal->GetActiveWeapon(), time, trace );

		//	emplace nade point
		m_Points.at( index++ ) = NadePoint_t( start, pos, trace.plane.vecNormal, trace.flFraction != 1.0f, true, detonate);
		start = pos;

		//	apply gravity modifier
		thrown_direction.z -= gravity * trace.flFraction * step;

		if( detonate ) {
			break;
		}
	}

	//	invalidate all empty points and finish prediction
	for( auto n = index; n < 500; ++n ) {
		m_Points.at( n ).m_bValid = false;
	}

	m_bPredicted = true;
}

bool Features::GrenadePrediction::Detonated(CWeapon* pWeapon, float flTime, trace_t& Trace)
{
	if( !pWeapon ) {
		return true;
	}

	//	get weapon item index
	const auto index = pWeapon->m_iItemDefinitionIndex();

	switch( index ) {
		//	flash and HE grenades only live up to 2.5s after thrown
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		if( flTime > 2.5f ) {
			return true;
		}
		break;

		//	fire grenades detonate on ground hit, or 3.5s after thrown
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		if( Trace.flFraction!= 1.0f && Trace.plane.vecNormal.z > 0.7f || flTime > 3.5f ) {
			return true;
		}
		break;

		//	decoy and smoke grenades were buggy in prediction, so i used this ghetto work-around
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		if( flTime > 2.5f ) {
			return true;
		}
		break;
	}

	return false;
}

void Features::GrenadePrediction::Trace()
{
	if(!Vars::Visuals::World::m_bGrenadePrediction)
		return;

	if( !( Client::m_pCmd->m_nButtons & IN_ATTACK ) && !( Client::m_pCmd->m_nButtons & IN_ATTACK2 ) ) {
		m_bPredicted = false;
		return;
	}

	const static std::vector< int > nades{
		WEAPON_FLASHBANG,
		WEAPON_SMOKEGRENADE,
		WEAPON_HEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE
	};

	//	grab local weapon
	auto weapon = Client::m_pLocal->GetActiveWeapon();

	if( !weapon ) {
		return;
	}

	if( std::find( nades.begin( ), nades.end( ), weapon->m_iItemDefinitionIndex( ) ) != nades.end( ) ) {
		return Predict();
	}

	m_bPredicted = false;
}

void Features::GrenadePrediction::Draw()
{
	if( !Vars::Visuals::World::m_bGrenadePrediction)
		return;

	if( !Interfaces::m_pEngine->IsInGame() || !Client::m_pLocal || !Client::m_pLocal->IsAlive())
		return;

	auto draw_3d_dotted_circle = [ ]( Vector position, float points, float radius ) {
		float step = M_PI * 2.0f / points;
		for( float a = 0; a < M_PI * 2.0f; a += step ) {
			Vector start( radius * cosf( a ) + position.x, radius * sinf( a ) + position.y, position.z );

			ImVec2 start2d;
			if( Renderer::WorldToScreen( start, start2d ) )
				Renderer::Line(start2d.x, start2d.y, start2d.x + 1, start2d.y + 1, Color( 255, 255, 255 ));
		}
	};

	ImVec2 start, end;

	//	draw nade path
	if(m_bPredicted ) {
		for( auto &p : m_Points ) {
			if( !p.m_bValid ) {
				break;
			}

			if( Renderer::WorldToScreen( p.m_vecStart, start ) && Renderer::WorldToScreen( p.m_vecEnd, end ) ) {
				//	draw line
				Renderer::Line(start.x, start.y, end.x, end.y, Color( 0, 125, 255 ));

				//	draw small box if detonated or hit a wall
				if( p.m_bDetonate || p.m_bPlane ) {
					Renderer::Rect(ImVec2(start.x - 2, start.y - 2),
						ImVec2(5, 5), p.m_bDetonate ? Color(255, 0, 0) : Color(255, 255, 255), 5.f);
				}

				if( p.m_bDetonate )
						draw_3d_dotted_circle( p.m_vecEnd, 100, 150 );
			}
		}
	}
}