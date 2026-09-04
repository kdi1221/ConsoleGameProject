#pragma once

#include <Math/Color.h>
#include <Math/Vector2Int.h>
#include <Resource/ResourceBase.h>
#include <vector>
#include <string>

/* Boss Eye 이미지 */
class ResourceBossEye : public Craft::ResourceBase
{
	// 타입 정보 설정을 위한 매크로 추가.
	TYPE_DECLARATIONS(ResourceBossEye, ResourceBase)

public:
	//눈 이미지 한라인당 구성요소
	struct FEyeLineImage
	{
		Craft::Vector2Int centerOffset = Craft::Vector2Int(0, 0);
		std::wstring image;
	};

	//눈의 구성요소에 대한 이미지 정보
	struct FEyeImage
	{
		/* 각 라인별 눈 구성요소 이미지 */
		std::vector<FEyeLineImage> lineImages;

		/* 이미지 색상 */
		Craft::Color drawColor = Craft::Color::White;
	};

	enum class eEyeImageCategory
	{
		//외곽
		Outline,

		//눈흰자(공막)
		Sclera,

		//홍채
		Iris,

		//동공
		Pupil,

		MaxNum
	};

public:
	ResourceBossEye();
	virtual ~ResourceBossEye();

public:
	/* 초기화 */
	virtual bool Initialize() override;

public:
	const FEyeImage& GetEyeImage(eEyeImageCategory imageCategory) const;

private:
	//눈 구성요소 이미지들
	std::vector<FEyeImage> eyeImages;
};

