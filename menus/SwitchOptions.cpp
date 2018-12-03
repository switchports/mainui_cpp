#ifdef __SWITCH__
#include "../../engine/platform/switch/overclocking_switch.h" // h
#include "Framework.h"
#include "Table.h"
#include "CheckBox.h"
#include "keydefs.h"
#include "TabView.h"

class CSwitchOverclockingModel : public CMenuBaseModel
{
public:
	void Update();
	int GetColumns() const { return 1; }
	int GetRows() const { return m_ocNumModes; }
	const char *GetCellText(int line, int column) { return m_ocModes[line]; }
private:
	int m_ocNumModes;
	const char *m_ocModes[64];
};

class CSwitchOptions: public CMenuFramework
{
private:
	void _Init( void ) override;
    void _VidInit();

public:
	typedef CMenuFramework BaseClass;
	CSwitchOptions() : CMenuFramework("CSwitchOptions") { }
    void SetConfig();

	CMenuTable overclockList;
	CSwitchOverclockingModel overclockListModel;
};

static CSwitchOptions	uiOptions;

void CSwitchOverclockingModel::Update( void )
{
    const size_t cpu_overclock_count = sizeof(SWITCH_CPU_OVERCLOCKING_SPEEDS) / sizeof(SWITCH_CPU_OVERCLOCKING_SPEEDS[1]);

    for (int i = 0; i < cpu_overclock_count; i++)
    {
        m_ocModes[i] = SWITCH_CPU_OVERCLOCKING_SPEEDS[i].title;
    }

    m_ocNumModes = cpu_overclock_count;
}

/*
=================
CSwitchOptions::Init
=================
*/
void CSwitchOptions::_Init( void )
{
	overclockList.SetRect( 360, 230, -20, 200 );
	overclockList.SetupColumn( 0, "Overclocking speed", 1.0f );
	overclockList.SetModel( &overclockListModel );

	AddItem( background );

	AddButton( "Apply", "Apply changes", PC_OK, VoidCb( &CSwitchOptions::SetConfig ) );
	AddButton( "Cancel", "Return back to previous menu", PC_CANCEL, VoidCb( &CSwitchOptions::Hide ) );

    AddItem( overclockList );
}

/*
=================
CSwitchOptions::_VidInit
=================
*/
void CSwitchOptions::_VidInit()
{
    float currentOverclock = EngFuncs::GetCvarFloat( "switch_overclock" );
    overclockList.SetCurrentIndex( currentOverclock );
}

/*
=================
CSwitchOptions::SetConfig
=================
*/
void CSwitchOptions::SetConfig()
{
    overclockingspeed_t overclock = SWITCH_CPU_OVERCLOCKING_SPEEDS[overclockList.GetCurrentIndex()];
    EngFuncs::CvarSetValue( "switch_overclock", overclock.id );
    EngFuncs::ClientCmd( TRUE, "switch_overclock_update\n" );

    CSwitchOptions::Hide(); // so people notice the settings has actually been applied lol
}

/*
=================
CSwitchOptions::Precache
=================
*/
void UI_Switch_Precache( void ) {}

/*
=================
CSwitchOptions::Menu
=================
*/
void UI_Switch_Menu( void )
{
	uiOptions.Show();
}
ADD_MENU( switch_options, UI_Switch_Precache, UI_Switch_Menu );
#endif