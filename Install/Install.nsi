;NSIS Script For Clocks-Sounds

!define MUI_PRODUCT "Clocks-Sounds" ;Define your own software name here
!define MUI_VERSION "1.70" ;Define your own software version here

!include "MUI.nsh"

;--------------------------------
;Configuration

	;Do A CRC Check
	CRCCheck On

	;General
	OutFile "${MUI_PRODUCT}-v${MUI_VERSION}-Install.exe"

	;Folder selection page
	InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"
	
	;Remember install folder
	InstallDirRegKey HKCU "Software\${MUI_PRODUCT}" ""
	
	;Remember the installer language
	!define MUI_LANGDLL_REGISTRY_ROOT "HKCU"
	!define MUI_LANGDLL_REGISTRY_KEY "Software\${MUI_PRODUCT}"
	!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Modern UI Configuration

	!define MUI_WELCOMEPAGE
	!define MUI_LICENSEPAGE
	!define MUI_COMPONENTSPAGE
		!define MUI_COMPONENTSPAGE_SMALLDESC
	!define MUI_DIRECTORYPAGE
	!define MUI_FINISHPAGE
		!define MUI_FINISHPAGE_RUN "$INSTDIR\ClockSnd.exe"
	
	!define MUI_ABORTWARNING
	
	!define MUI_UNINSTALLER
	!define MUI_UNCONFIRMPAGE

;--------------------------------
;Languages

	!insertmacro MUI_LANGUAGE "English"
	!insertmacro MUI_LANGUAGE "French"
	
;--------------------------------
;Language Strings
		
	;Sections
	LangString SecMain ${LANG_ENGLISH} "${MUI_PRODUCT} (required)"
	LangString SecMain ${LANG_FRENCH} "${MUI_PRODUCT} (requis)"

	LangString SecShortcuts ${LANG_ENGLISH} "Start Menu Shortcuts"
	LangString SecShortcuts ${LANG_FRENCH} "Raccourcis du Menu Démarrer"
	
	;Descriptions
	LangString DESC_SecMain ${LANG_ENGLISH} "The core files required to use ${MUI_PRODUCT}"
	LangString DESC_SecMain ${LANG_FRENCH} "Fichiers principaux requis pour utiliser ${MUI_PRODUCT}"

	LangString DESC_SecShortcuts ${LANG_ENGLISH} "Create Start Menu shortcuts"
	LangString DESC_SecShortcuts ${LANG_FRENCH} "Créé les raccourcis dans le menu démarrer"

;--------------------------------
;Data
	
	LicenseData /LANG=${LANG_ENGLISH} "License.txt"
	LicenseData /LANG=${LANG_FRENCH} "Licence.txt"

;--------------------------------
;Reserve Files
	
	;Things that need to be extracted on first (keep these lines before any File command!)
	;Only useful for BZIP2 compression
	!insertmacro MUI_RESERVEFILE_LANGDLL
	
;--------------------------------
;Installer Sections

Section "$(SecMain)" descSecMain
	SectionIn RO

	;Install Files
	SetOutPath "$INSTDIR"
	File "ClockSnd.exe"
	StrCmp $LANGUAGE ${LANG_ENGLISH} 0 +2
		File "License.txt"
	StrCmp $LANGUAGE ${LANG_FRENCH} 0 +2
		File "Licence.txt"
	File "History.txt"
	SetOutPath "$INSTDIR\Chime"
	File "..\chime\*.wav"
	SetOutPath "$INSTDIR\Alarm"
	File "..\alarm\*.wav"

	;Store install folder
	WriteRegStr HKCU "Software\${MUI_PRODUCT}" "" $INSTDIR
	
	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninst.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayIcon" "$INSTDIR\Uninst.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayVersion" "${MUI_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "Publisher" "Liberty's"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "HelpLink" "http://www.alc-wbc.com/"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "URLInfoAbout" "http://www.libertys.com/"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "URLUpdateInfo" "http://www.alc-wbc.com/"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "NoModify" 0x00000001
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "NoRepair" 0x00000001
	WriteUninstaller "$INSTDIR\Uninst.exe"
SectionEnd
	
Section "$(SecShortcuts)" descSecShortcuts
	;Add Shortcuts
	CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
	CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk" "$INSTDIR\ClockSnd.exe" "" "$INSTDIR\ClockSnd.exe" 0

	StrCmp $LANGUAGE ${LANG_ENGLISH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\License.lnk" "$INSTDIR\License.txt" "" "$INSTDIR\License.txt" 0
	StrCmp $LANGUAGE ${LANG_FRENCH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Licence.lnk" "$INSTDIR\Licence.txt" "" "$INSTDIR\Licence.txt" 0

	StrCmp $LANGUAGE ${LANG_ENGLISH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Change Log.lnk" "$INSTDIR\History.txt" "" "$INSTDIR\History.txt" 0
	StrCmp $LANGUAGE ${LANG_FRENCH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Historique.lnk" "$INSTDIR\History.txt" "" "$INSTDIR\History.txt" 0

	StrCmp $LANGUAGE ${LANG_ENGLISH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninst.exe" "" "$INSTDIR\Uninst.exe" 0
	StrCmp $LANGUAGE ${LANG_FRENCH} 0 +2
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Désinstaller.lnk" "$INSTDIR\Uninst.exe" "" "$INSTDIR\Uninst.exe" 0

	;Delete old versions shortcut
	Delete "$SMSTARTUP\${MUI_PRODUCT}.lnk"

	;New shortcut in registry
	WriteRegStr HKEY_CURRENT_USER "Software\Microsoft\Windows\CurrentVersion\Run" "${MUI_PRODUCT}" "$INSTDIR\ClockSnd.exe"
SectionEnd

;--------------------------------
;Installer Functions

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------
;Descriptions

!insertmacro MUI_FUNCTIONS_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${descSecMain} $(DESC_SecMain)
	!insertmacro MUI_DESCRIPTION_TEXT ${descSecShortcuts} $(DESC_SecShortcuts)
!insertmacro MUI_FUNCTIONS_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"
	;Delete Files
	Delete "$INSTDIR\ClockSnd.exe"
	Delete "$INSTDIR\ReadMe.txt"
	Delete "$INSTDIR\Chime\*.wav"
	Delete "$INSTDIR\Alarm\*.wav"

	;Delete Start Menu Shortcuts
	Delete "$SMPROGRAMS\${MUI_PRODUCT}\*.*"
	RmDir "$SMPROGRAMS\${MUI_PRODUCT}"
	DeleteRegValue HKEY_CURRENT_USER "Software\Microsoft\Windows\CurrentVersion\Run" "${MUI_PRODUCT}"

	;Delete Uninstaller And Unistall Registry Entries
	Delete "$INSTDIR\Uninst.exe"
	DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${MUI_PRODUCT}"
	DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"
	RmDir "$INSTDIR\Chime"
	RmDir "$INSTDIR\Alarm"
	RmDir "$INSTDIR"

	DeleteRegKey /ifempty HKCU "Software\${MUI_PRODUCT}"

	;Display the Finish header
	!insertmacro MUI_UNFINISHHEADER
SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit
	;Get language from registry
	ReadRegStr $LANGUAGE HKCU "Software\${MUI_PRODUCT}" "Installer Language"
FunctionEnd