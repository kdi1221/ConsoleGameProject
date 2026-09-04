#include "ResourceBossEye.h"
#include <cassert>

static const std::wstring oneEyeAsciiArts[] =
{
L"                             **********************                             ",
L"                         *****--------------------*****                         ",
L"                     *****----------------------------****                      ",
L"                  ****------------###########------------****                   ",
L"               ****------------#################-------------****               ",
L"            ****-------------######################-------------****            ",
L"          ****--------------########################--------------****          ",
L"       ****----------------##########################----------------****       ",
L"     ***------------------###########@@@@@@###########------------------***     ",
L"  ****--------------------#########@@@@@@@@@@#########--------------------****  ",
L"***-----------------------#########@@@@@@@@@@#########-----------------------***",
L"*-------------------------#########@@@@@@@@@@#########-------------------------*",
L"***-----------------------###########@@@@@@###########-----------------------***",
L"  ****---------------------##########################---------------------****  ",
L"     ***--------------------########################--------------------***     ",
L"       ***-------------------######################-------------------***       ",
L"         ***-------------------##################-------------------***         ",
L"            ****------------------############------------------****            ",
L"               ****------------------------------------------****               ",
L"                  ****------------------------------------****                  ",
L"                     ****------------------------------****                     ",
L"                        ******--------------------******                        ",
L"                            ************************                            "
};

using namespace Craft;

ResourceBossEye::ResourceBossEye()
	:eyeImages(static_cast<int>(eEyeImageCategory::MaxNum))
{

}

ResourceBossEye::~ResourceBossEye()
{

}

bool ResourceBossEye::Initialize()
{
	const Vector2Int centerPos = Vector2Int(40, _countof(oneEyeAsciiArts) >> 1);

	const size_t eyeAsciiArtsHeight = _countof(oneEyeAsciiArts);
	
	auto initializeEyeImagesBuffer = [this](eEyeImageCategory imageCategory, Color color)
		{
			eyeImages[static_cast<int>(imageCategory)].lineImages.reserve(eyeAsciiArtsHeight);
			eyeImages[static_cast<int>(imageCategory)].drawColor = color;
		};

	initializeEyeImagesBuffer(eEyeImageCategory::Outline, Color::DarkGray);
	initializeEyeImagesBuffer(eEyeImageCategory::Sclera, Color::BrightWhite);
	initializeEyeImagesBuffer(eEyeImageCategory::Iris, Color::Red);
	initializeEyeImagesBuffer(eEyeImageCategory::Pupil, Color::Black);

	auto fillEyeImage = [&centerPos](int xMin, int xMax, FEyeLineImage& eyeImages, int yCoordinate, wchar_t fillChar)
		{
			const int width = (xMax - xMin) + 1;
			eyeImages.image.resize(width);
			for (int xPos = xMin; xPos <= xMax; ++xPos)
			{
				const int imageIndex = xPos - xMin;
				eyeImages.image[imageIndex] = fillChar;
			}

			eyeImages.centerOffset = Vector2Int(xMin, yCoordinate) - centerPos;
		};

	int yCoordinate = 0;
	for (const std::wstring& oneEyeAsciiLine : oneEyeAsciiArts)
	{
		int xCoordinate = 0;

		int outlineXMin = INT_MAX, outlineXMax = INT_MIN;
		int scleraXMin = INT_MAX, scleraXMax = INT_MIN;
		int irisXMin = INT_MAX, irisXMax = INT_MIN;
		int pupilXMin = INT_MAX, pupilXMax = INT_MIN;
	
		for (const wchar_t asciiChar : oneEyeAsciiLine)
		{
			switch (asciiChar)
			{
			case '*':
				{
					//외곽
					outlineXMin = min(outlineXMin, xCoordinate);
					outlineXMax = max(outlineXMax, xCoordinate);
				}
				break;

			case '-':
				{
					//눈 흰자(공막)
					scleraXMin = min(scleraXMin, xCoordinate);
					scleraXMax = max(scleraXMax, xCoordinate);
				}
				break;

			case '#':
				{
					//홍채
					irisXMin = min(irisXMin, xCoordinate);
					irisXMax = max(irisXMax, xCoordinate);
				}
				break;

			case '@':
				{
					//동공
					pupilXMin = min(pupilXMin, xCoordinate);
					pupilXMax = max(pupilXMax, xCoordinate);
				}
				break;
			}

			++xCoordinate;
		}

		if (outlineXMin != INT_MAX && outlineXMax != INT_MIN)
		{
			std::vector<FEyeLineImage>& eyeOutlineImages = eyeImages[static_cast<int>(eEyeImageCategory::Outline)].lineImages;

			eyeOutlineImages.emplace_back(FEyeLineImage());
			fillEyeImage(outlineXMin, outlineXMax, eyeOutlineImages[eyeOutlineImages.size() - 1], yCoordinate, L'█');
		}

		if (scleraXMin != INT_MAX && scleraXMax != INT_MIN)
		{
			std::vector<FEyeLineImage>& eyeScleraImages = eyeImages[static_cast<int>(eEyeImageCategory::Sclera)].lineImages;

			eyeScleraImages.emplace_back(FEyeLineImage());
			fillEyeImage(scleraXMin, scleraXMax, eyeScleraImages[eyeScleraImages.size() - 1], yCoordinate, L'█');
		}

		/* 홍채와 동공은 원본 이미지가 한칸 위로 올라가 있어서 한칸 내림 */
		if (irisXMin != INT_MAX && irisXMax != INT_MIN)
		{
			std::vector<FEyeLineImage>& eyeIrisImages = eyeImages[static_cast<int>(eEyeImageCategory::Iris)].lineImages;

			eyeIrisImages.emplace_back(FEyeLineImage());
			fillEyeImage(irisXMin, irisXMax, eyeIrisImages[eyeIrisImages.size() - 1], yCoordinate + 1, L'█');
		}

		if (pupilXMin != INT_MAX && pupilXMax != INT_MIN)
		{
			std::vector<FEyeLineImage>& eyePupilImages = eyeImages[static_cast<int>(eEyeImageCategory::Pupil)].lineImages;

			eyePupilImages.emplace_back(FEyeLineImage());
			fillEyeImage(pupilXMin, pupilXMax, eyePupilImages[eyePupilImages.size() - 1], yCoordinate + 1, L'█');
		}

		++yCoordinate;
	}

	return true;
}

const ResourceBossEye::FEyeImage& ResourceBossEye::GetEyeImage(eEyeImageCategory imageCategory) const
{
	const int imageIndex = static_cast<int>(imageCategory);
	const bool bValidIndex = imageIndex >= 0 && imageIndex < static_cast<int>(eyeImages.size());
	assert(bValidIndex && "Invalid Image Index..");
	
	return eyeImages[imageIndex];
}