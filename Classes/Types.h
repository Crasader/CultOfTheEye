#pragma once
#include <string>
#include <array>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

//For some Trigonometry
#define PI 3.1415

namespace ScaleConstants
{
	const float CORE_SCALE = 0.75;
	const float RING_SCALE = 3.75;
	const float FOG_SCALE = 3.75;
	const int TOWER_SCALE = 1;
	const int CORE_RADIUS = 1;
	const int RING_RADIUS = 925;
	const int TOP_QUADRANT_Y_OFFSET = 0;
	const int BOTTOM_QUADRANT_Y_OFFSET = 0;
	const int SLOT_ANGLE = 15;
}

namespace AnimationConstants
{
	const float RING_SPEED = (1/10.f);
	const float FOG_SPEED = (1/15.f);
}

enum eCommonColors
{
	DEFAULT = 0,
	RED,
	GREEN,
	BLUE
};

enum eTowerType
{
	NORMAL,
	SHIELD,
	TOWER_COUNT
};

enum Quadrants
{
	A = 0,
	B, C, D,
	QuadrantCount
};

enum TowerCategory
{
	Offense = 1 << 0,
	Defense = 1 << 1
};

enum eTowerState
{
	COOLDOWN = 0,
	ACTIVE
};

//Identifiers for Arrow Class
namespace ProjectileConstants
{
	const int NormalImpulse = 17500;
	const int NormalGravity = -6;
	const int ShieldImpulse = 10000;
	const int ShieldGravity = -9;
}

namespace UpgradeCosts
{
	const int OffenseL1 = 3;
	const int DefenseL1 = 3;
	const float UpgradeTime = 1;
}

namespace Animations
{
	namespace Monster
	{
		namespace Head
		{
			const string fIdle = "headIdle%02d%02d.png";
			const string fSpray = "headSpray%02d%02d.png";
		}
		namespace Tentacle
		{
			const string fIdle = "tentacleIdle%02d%02d.png";
			const string fAttack = "tentacleAttack%02d%02d.png";
			const string fDamaged = "tentacleDamaged%02d%02d.png";
			const string fRetreat = "tentacleRetreat%02d%02d.png";
		}
		namespace Swim
		{
			const string fIdle = "swim%02d%02d.png";
		}
	}
	namespace Essence
	{
		const string fIdle = "essence%02d.png";
	}
}

namespace FilePaths
{
	const string AttackTower = "Images/_attackTower.png";
	const string AttackProjectile = "Images/_attackProjectile.png";
	const string AttackBallista = "Images/_attackBallista.png";
	const string AttackDirMarker = "Images/_attackDirMarker.png";

	const string ShieldTower = "Images/_shieldTower.png";
	const string ShieldProjectile = "Images/_shieldProjectile.png";
	const string ShieldBallista = "Images/_shieldBallista.png";
	const string ShieldDirMarker = "Images/_shieldDirMarker.png";

	const string AttackTowerButton = "Images/TowerMenu/Attack.png";
	const string ShieldTowerButton = "Images/TowerMenu/Shield.png";

	namespace UI
	{
		const string ResourceBar	= "Images/Game UI/essenceIndicator.png";
		const string MonsterHPBar	= "Images/Game UI/monsterHealth.png";
		const string Victory		= "Images/UI/victory.png";
		const string Defeat			= "Images/UI/defeat.png";

		namespace Buttons
		{
			const string FilePath = "Images/UI/Buttons/";

			const string NextNormal =	FilePath + "_normalNext.png";
			const string NextSelected = FilePath + "_selectedNext.png";
			const string BackNormal =	FilePath + "_normalBack.png";
			const string BackSelected = FilePath + "_selectedBack.png";
			const string MenuNormal =	FilePath + "_normalMenu.png";
			const string MenuSelected = FilePath + "_selectedMenu.png";
			const string PlayNormal =	FilePath + "_normalPlay.png";
			const string PlaySelected = FilePath + "_selectedPlay.png";
			const string CreditsNormal =	FilePath + "_normalCredits.png";
			const string CreditsSelected =	FilePath + "_selectedCredits.png";
			const string ControlsNormal =	FilePath + "_normalControls.png";
			const string ControlsSelected = FilePath + "_selectedControls.png";
		}
	}

	namespace Spritesheets
	{
		namespace Monster
		{
			const string FilePath = "Images/Monster/";
			
			namespace Head
			{
				const int Count = 12;
				const string FilePath = Monster::FilePath + "Head/";

				const array<const string, Count> Strings =
				{
					FilePath + "headIdle/headIdle1_1.plist",
					FilePath + "headIdle/headIdle1_2.plist",
					FilePath + "headIdle/headIdle2_1.plist",
					FilePath + "headIdle/headIdle2_2.plist",
					FilePath + "headIdle/headIdle3_1.plist",
					FilePath + "headIdle/headIdle3_2.plist",

					FilePath + "headSpray/headSpray010.plist",
					FilePath + "headSpray/headSpray011.plist",
					FilePath + "headSpray/headSpray020.plist",
					FilePath + "headSpray/headSpray021.plist",
					FilePath + "headSpray/headSpray030.plist",
					FilePath + "headSpray/headSpray031.plist"
				};
			}
			namespace Tentacle
			{
				const int Count = 31;
				const string FilePath = Monster::FilePath + "Tentacle/";

				const array<const string, Count> Strings =
				{
					FilePath + "tentacleIdle/tentacleIdle010.plist",
					FilePath + "tentacleIdle/tentacleIdle011.plist",
					FilePath + "tentacleIdle/tentacleIdle012.plist",
					FilePath + "tentacleIdle/tentacleIdle020.plist",
					FilePath + "tentacleIdle/tentacleIdle021.plist",
					FilePath + "tentacleIdle/tentacleIdle022.plist",
					FilePath + "tentacleIdle/tentacleIdle030.plist",
					FilePath + "tentacleIdle/tentacleIdle031.plist",
					FilePath + "tentacleIdle/tentacleIdle032.plist",

					FilePath + "tentacleAttack/tentacleAttack010.plist",
					FilePath + "tentacleAttack/tentacleAttack011.plist",
					FilePath + "tentacleAttack/tentacleAttack012.plist",
					FilePath + "tentacleAttack/tentacleAttack020.plist",
					FilePath + "tentacleAttack/tentacleAttack021.plist",
					FilePath + "tentacleAttack/tentacleAttack030.plist",
					FilePath + "tentacleAttack/tentacleAttack031.plist",

					FilePath + "tentacleDamaged/tentacleDamaged010.plist",
					FilePath + "tentacleDamaged/tentacleDamaged011.plist",
					FilePath + "tentacleDamaged/tentacleDamaged012.plist",
					FilePath + "tentacleDamaged/tentacleDamaged020.plist",
					FilePath + "tentacleDamaged/tentacleDamaged021.plist",
					FilePath + "tentacleDamaged/tentacleDamaged022.plist",
					FilePath + "tentacleDamaged/tentacleDamaged030.plist",
					FilePath + "tentacleDamaged/tentacleDamaged031.plist",
					FilePath + "tentacleDamaged/tentacleDamaged032.plist",

					FilePath + "tentacleRetreat/tentacleRetreat010.plist",
					FilePath + "tentacleRetreat/tentacleRetreat011.plist",
					FilePath + "tentacleRetreat/tentacleRetreat020.plist",
					FilePath + "tentacleRetreat/tentacleRetreat021.plist",
					FilePath + "tentacleRetreat/tentacleRetreat030.plist",
					FilePath + "tentacleRetreat/tentacleRetreat031.plist"
				
				};
			}

			namespace Swim
			{
				const int Count = 9;
				const string FilePath = Monster::FilePath + "Swim/";

				const array<const string, Count> Strings =
				{
					FilePath + "swim010.plist",
					FilePath + "swim011.plist",
					FilePath + "swim012.plist",
					FilePath + "swim020.plist",
					FilePath + "swim021.plist",
					FilePath + "swim022.plist",
					FilePath + "swim030.plist",
					FilePath + "swim031.plist",
					FilePath + "swim032.plist"
				};
			}
		}

		namespace Essence
		{
			const string FilePath = "Images/Game UI/essence.plist";
		}
	}

	namespace Physics
	{
		const string FilePath = "Physics/";

		namespace Monster
		{
			namespace Tentacle
			{
				const int Count = 4;

				const array<const pair<const string, const string>, Count> Strings =
				{
					make_pair("tentacleIdle",		Physics::FilePath + "tentacleIdle.plist"),
					make_pair("tentacleAttack",		Physics::FilePath + "tentacleAttack.plist"),
					make_pair("tentacleDamaged",	Physics::FilePath + "tentacleDamaged.plist"),
					make_pair("tentacleRetreat",	Physics::FilePath + "tentacleRetreat.plist")
				};
			}

			namespace Head
			{
				const int Count = 2;

				const array<const pair<const string, const string>, Count> Strings =
				{
					make_pair("headIdle",	Physics::FilePath + "headIdle.plist"),
					make_pair("headSpray",	Physics::FilePath + "headSpray.plist")
				};
			}

			namespace Swim
			{
				const int Count = 1;

				const array<const pair<const string, const string>, Count> Strings =
				{
					make_pair("swimIdle",	Physics::FilePath + "swim.plist")
				};
			}
		}
	}

	namespace Background
	{
		const string FilePath	= "Images/UI/";

		const string Title		= FilePath + "backgroundIntro.png";
		const string Credits	= FilePath + "backgroundCredits.png";
		const string Tutorial	= FilePath + "backgroundTutorial.png";

		const string End		= FilePath + "backgroundEnd.png";
	}
		
	namespace Audio
	{
		namespace Music
		{
			const string FilePath = "Audio/Music/";

			const string WinBackground = FilePath + "GameSceneDefault.mp3";
			const string AndroidBackground = FilePath + "GameSceneDefault.ogg";
		}

		namespace SFX
		{
			const string FilePath = "Audio/SFX/";

			namespace Monster
			{
				const string FilePath = SFX::FilePath + "Monster/";

				const string Hurt = FilePath + "Hurt.ogg";
				const string Idle1 = FilePath + "Idle1.ogg";
				const string Idle2 = FilePath + "Idle2.ogg";
				const string Idle3 = FilePath + "Idle3.ogg";
				const string Idle4 = FilePath + "Idle4.ogg";
			}

			namespace Tower
			{
				const string FilePath = SFX::FilePath + "Tower/";

				const string Damaged = FilePath + "Damaged.ogg";
				const string Destroyed = FilePath + "Destroyed.ogg";
				const string Shoot = FilePath + "Shoot.ogg";
			}
		}
	}

	namespace Fonts
	{
		const string StandardFont = "Fonts/CultFont.ttf";
	}

	const string UITutorialControls = "Images/UI/_tutorialControls.png";

}

namespace ClassStrings
{
	const string Projectile = "Projectile";
	const string Drop = "Drop";
	const string Core = "Core";
	const string Tower = "Tower";
	const string Debris = "Debris";
	const string Monster = "Monster";
	const string TowerManager = "TowerManager";
};
