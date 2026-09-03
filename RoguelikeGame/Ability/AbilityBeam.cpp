//#include "AbilityBeam.h"
//#include "Actor/Pawn/Pawn.h"
//#include "StaticLibrary/StaticFunctionLibrary.h"
//#include "Navigation/NavigationTilemap.h"
//#include "Engine/Engine.h"
//#include "Render/Renderer.h"
//#include "Level/TilemapLevel.h"
//#include <vector>
//#include <Engine/Engine.h>
//
//using namespace Craft;
//
//static constexpr Color displayColorList[] = { Color::BG_Red, Color::BG_LightPurple };
//
//AbilityBeam::AbilityBeam(int level, eTeamID teamID)
//	:super(2, level)
//	,instigatorTeamID(teamID)
//{
//	timerColorUpdate.SetTargetTime(0.05f);
//
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].SetTargetTime(0.02f);
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].SetTargetTime(0.2f);
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].SetTargetTime(1.f);
//}
//
//void AbilityBeam::Tick(float deltaTime)
//{
//	timerColorUpdate.Tick(deltaTime);
//	if (timerColorUpdate.IsTimeOut())
//	{
//		//색상 업데이트
//		ChangeDisplayColor();
//
//		timerColorUpdate.Reset();
//	}
//
//	bool bChangeRange = false;
//
//	switch (beamMode)
//	{
//	case eBeamMode::Growth:
//		{
//			timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].Tick(deltaTime);
//			if (timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].IsTimeOut())
//			{
//				timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].Reset();
//
//				//길이 증가
//				beamCurrentRange = min(beamCurrentRange + 1.f, beamMaxRange);
//
//				bChangeRange = true;
//
//				if (beamCurrentRange >= beamMaxRange)
//				{
//					DamageProcess();
//
//					beamMode = eBeamMode::Keep;
//					timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].Reset();
//				}
//			}
//		}
//		break;
//
//	case eBeamMode::Keep:
//		{
//			timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].Tick(deltaTime);
//			if (timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].IsTimeOut())
//			{
//				timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].Reset();
//
//				//길이 초기화
//				beamCurrentRange = 0.f;
//
//				bChangeRange = true;
//
//				timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].Reset();
//				beamMode = eBeamMode::lull;
//			}
//		}
//		break;
//
//	case eBeamMode::lull:
//		{
//			timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].Tick(deltaTime);
//			if (timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].IsTimeOut())
//			{
//				timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].Reset();
//
//				std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
//				if (ownerPawn)
//				{
//					const Vector2Int startPos = ownerPawn->GetWorldPosition();
//					lastInputDirection = ownerPawn->GetAimingDirection();
//
//					BuildBeamPaths(startPos);
//				}
//
//				timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].Reset();
//				beamMode = eBeamMode::Growth;
//
//				Engine::Get().PlayOneShot("Effect/cast.wav");
//			}
//		}
//		break;
//	}
//
//	//경로 업데이트(직전 경로 업데이트 위치와 방향하고 다를때만)
//	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
//	if (ownerPawn)
//	{
//		const Vector2Int startPos = ownerPawn->GetWorldPosition();
//
//		if (startPos != prevBeamPathStartPos || bChangeRange)
//		{
//			BuildBeamPaths(startPos);
//		}
//	}
//
//}
//
//void AbilityBeam::Draw()
//{
//	super::Draw();
//
//	for (const Vector2Int& path : beamPaths)
//	{
//		Renderer::Get().Submit(L" ", path, displayColorList[displayColorIndex], static_cast<int>(eRenderSortingOrder::beam));
//	}
//}
//
//void AbilityBeam::TriggerOn()
//{
//	super::TriggerOn();
//
//	beamMode = eBeamMode::Growth;
//	beamCurrentRange = 0.f;
//	displayColorIndex = 1;
//	timerColorUpdate.ReserveNextTick();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].ReserveNextTick();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].Reset();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].Reset();
//
//	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
//	if (ownerPawn)
//	{
//		const Vector2Int startPos = ownerPawn->GetWorldPosition();
//		lastInputDirection = ownerPawn->GetAimingDirection();
//
//		BuildBeamPaths(startPos);
//	}
//
//	Engine::Get().PlayOneShot("Effect/cast.wav");
//}
//
//void AbilityBeam::TriggerOff()
//{
//	super::TriggerOff();
//
//	prevBeamPathStartPos = Vector2Int(-1, -1);
//	lastInputDirection = Vector2Float::Zero;
//
//	beamPaths.clear();
//	beamMode = eBeamMode::Growth;
//	beamCurrentRange = 0.f;
//	displayColorIndex = 0;
//	timerColorUpdate.Reset();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Growth)].Reset();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Keep)].Reset();
//	timerRangeUpdate[static_cast<unsigned int>(eBeamMode::lull)].Reset();
//}
//
//void AbilityBeam::BuildBeamPaths(const Vector2Int& startPos)
//{
//	if (lastInputDirection.IsNearlyZero())
//	{
//		return;
//	}
//
//	static const float spreadAngle = 20.f;
//
//	beamPaths.clear();
//
//	if (beamCurrentRange > 0.f)
//	{
//		auto addBeamPathFollowDirection = [&](const Vector2Float& followDirection)
//			{
//				if (followDirection.IsNearlyZero())
//				{
//					return;
//				}
//
//				std::vector<float> fireAngles = { 0.f };
//				for (int i = 1; i < 3; ++i)
//				{
//					const float currentSpreadAngle = spreadAngle * i;
//					fireAngles.push_back(-currentSpreadAngle);
//					fireAngles.push_back(+currentSpreadAngle);
//				}
//
//				for (float fireAngle : fireAngles)
//				{
//					const Vector2Float rotateAimingDir = Vector2Float::RotateVector(followDirection, fireAngle);
//					const Vector2Float beamDestinationPos = static_cast<Vector2Float>(startPos) + rotateAimingDir * beamCurrentRange;
//					const Vector2Int endPos(static_cast<int>(round(beamDestinationPos.x)), static_cast<int>(round(beamDestinationPos.y)));
//
//					/* 브레젠험 알고리즘으로 경로를 구한다.*/
//					std::vector<Vector2Int> currentPaths;
//					StaticFunctionLibrary::GetBresenhamPath(startPos, endPos, currentPaths);
//
//					const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();
//					for (auto iterBeamPath = currentPaths.begin(); iterBeamPath != currentPaths.end(); ++iterBeamPath)
//					{
//						eTileCategory pathTileCategory = navigationSystem.GetTileCategory(*iterBeamPath);
//						if (eTileCategory::Ground != pathTileCategory)
//						{
//							break;
//						}
//
//						/* Ground 타일 위치만 추가한다. */
//						beamPaths.insert(*iterBeamPath);
//					}
//				}
//			};
//
//		const int currentLevel = GetAbilityLevel();
//		std::vector<float> directionAngles;
//		switch (currentLevel)
//		{
//		case 3:
//			directionAngles.emplace_back(-90.f);
//			directionAngles.emplace_back(90.f);
//		case 2:
//			directionAngles.emplace_back(180.f);
//		case 1:
//			directionAngles.emplace_back(0.f);
//			break;
//		}
//
//		for (float angle : directionAngles)
//		{
//			addBeamPathFollowDirection(Vector2Float::RotateVector(lastInputDirection, angle));
//		}
//	}
//
//	/* 생성한 경로 정보를 저장해둔다. */
//	prevBeamPathStartPos = startPos;
//}
//
//void AbilityBeam::ChangeDisplayColor()
//{
//	displayColorIndex = 1 - displayColorIndex;
//}
//
//void AbilityBeam::DamageProcess()
//{
//	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
//	if (!ownerPawn)
//	{
//		return;
//	}
//
//	std::shared_ptr<TilemapLevel> currentTilemap = Cast<TilemapLevel>(ownerPawn->GetOwner());
//	if (!currentTilemap)
//	{
//		return;
//	}
//
//	for (const Vector2Int& path : beamPaths)
//	{
//		currentTilemap->AddDamageInfoToTile(path, damageValue, instigatorTeamID);
//	}
//}
