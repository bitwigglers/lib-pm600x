'
' Erstellt mit SharpDevelop.
' Benutzer: AHaupt
' Datum: 06.03.2009
' Zeit: 11:37
' 
' Sie können diese Vorlage unter Extras > Optionen > Codeerstellung > Standardheader ändern.
'


Public Class PM6006
	
	Const TYPE_PM_6006 As UInt32 = &H1f180400
	Const TYPE_PM_6003 As UInt32 = &H1f180401
	Const TYPE_PM_ALL  As UInt32 = 0

	Structure pm_context
		Public handle As IntPtr
		Public type As UInt32
		Public frequency As UInt64
		Public averages As UInt16
		Public error_str As IntPtr
	End Structure	
                                                               
    Public Declare Function pm_init _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context) _
                                As Integer

    Public Declare Function pm_open _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context, _
                                ByVal serial As UInt32) _
                                As Integer

    Public Declare Function pm_close _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context) _
                                As Integer
                                
	Public Declare Function pm_reset _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context) _
                                As Integer

	Public Declare Function pm_blink _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context) _
                                As Integer

	Public Declare Function pm_type _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context, _
                                ByRef type As UInt32) _
                                As Integer

    Public Declare Function pm_set_frequency _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context, _
                                ByVal freq As UInt64) _
                                As Integer

    Public Declare Function pm_set_averages _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context, _
                                ByVal avg As UInt16) _
                                As Integer
                                
    Public Declare Function pm_measure _
                                Lib "libpm600x.dll" ( _
                                ByRef pm As pm_context, _
                                ByRef freq As Single) _
                                As Integer
    
End Class
