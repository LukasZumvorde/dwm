/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
//static const char *fonts[]          = { "monospace:size=10" };
static const char *fonts[]          = { "DejavuSandMono:semibold:size=10" };
//static const char dmenufont[]       = "monospace:size=10";
static const char dmenufont[]       = "DejavuSandMono:semibold:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_orange[]      = "#ff9900";

static const char col_nord00[]      = "#2e3440";
static const char col_nord01[]      = "#3b4252";
static const char col_nord02[]      = "#434c5e";
static const char col_nord03[]      = "#4c566a";
static const char col_nord04[]      = "#d8dee9";
static const char col_nord05[]      = "#e5e9f0";
static const char col_nord06[]      = "#eceff4";
static const char col_nord07[]      = "#8fbcbb";
static const char col_nord08[]      = "#88c0d0";
static const char col_nord09[]      = "#81a1c1";
static const char col_nord10[]      = "#5e81ac";
static const char col_nord11[]      = "#bf616a";
static const char col_nord12[]      = "#d08770";
static const char col_nord13[]      = "#ebcb8b";
static const char col_nord14[]      = "#a3be8c";
static const char col_nord15[]      = "#b48ead";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_nord05, col_nord01, col_nord01 },
	[SchemeSel]  = { col_nord00, col_nord08, col_nord08 },
  [SchemeHid]  = { col_nord09, col_nord00, col_nord01 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "|M|",      centeredmaster },
  { "TTT",      bstack },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *roficmd[]     = { "rofi", "-modi drun,run,ssh,window" , "-show", "drun", "-show-icons", NULL };
static const char *rofipasscmd[] = { "rofi-pass", NULL };
static const char *roficlipcmd[] = { "rofi", "-modi", "clipboard:greenclip print", "-show", "clipboard", NULL };
static const char *dmenucmd[]    = { "dmenu_recency", "-l", "10", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nord01, "-nf", col_nord05, "-sb", col_nord08, "-sf", col_nord00, NULL };
static const char *termcmd[]     = { "alacritty", "-e", "tmux", NULL };
static const char *editorcmd[]   = { "emacsclient", "-c", "-a", "emacs", NULL };
static const char *fmcmd[]       = { "pcmanfm", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *emailcmd[]    = { "thunderbird", NULL };
static const char *musiccmd[]    = { "alacritty", "-e", "mocp", NULL };
static const char *passcmd[]     = { "passmenu", "-l", "30", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nord01, "-nf", col_nord05, "-sb", col_nord08, "-sf", col_nord00, NULL };
static const char *typepasscmd[] = { "passmenu", "--type", "-l", "30", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nord01, "-nf", col_nord05, "-sb", col_nord08, "-sf", col_nord00, NULL };
static const char *clipcmd[]     = { "clipmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nord01, "-nf", col_nord05, "-sb", col_nord08, "-sf", col_nord00, NULL };
static const char *brightness_inc[] = { "xbacklight", "-inc", "10", NULL };
static const char *brightness_dec[] = { "xbacklight", "-dec", "10", NULL };
static const char *screenlock[]  = { "i3lock", "-c" , "000000" , NULL };
static const char *shutdowncmd[] = { "shutdown", "-h", "now" , NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_F1,     spawn,          {.v = editorcmd } },
	{ MODKEY,                       XK_F2,     spawn,          {.v = fmcmd } },
	{ MODKEY,                       XK_F3,     spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_F4,     spawn,          {.v = emailcmd } },
	{ MODKEY,                       XK_F5,     spawn,          {.v = musiccmd } },
	{ MODKEY,                       XK_F6,     spawn,          {.v = rofipasscmd } },
	{ MODKEY|ShiftMask,             XK_F6,     spawn,          {.v = typepasscmd } },
	{ MODKEY,                       XK_F7,     spawn,          {.v = roficlipcmd } },
	{ MODKEY,                       XK_d,      spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_odiaeresis,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[3]} },
  { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
  { MODKEY,                       XK_plus,   setgaps,        {.i = +1 } },
  { MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = 0  } },
  { MODKEY|ShiftMask,             XK_plus,   setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = shutdowncmd} },
  { MODKEY|ShiftMask,             XK_r,      quit,           {0} },
  { MODKEY|Mod1Mask,              XK_q,      spawn,          {.v = screenlock} },
  { 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = brightness_inc} },
  { 0,          XF86XK_MonBrightnessDown,    spawn,          {.v = brightness_dec} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
