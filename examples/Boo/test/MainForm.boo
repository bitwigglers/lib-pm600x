namespace test

import System
import System.Collections
import System.Drawing
import System.Windows.Forms

partial class MainForm:
	def constructor():
		// The InitializeComponent() call is required for Windows Forms designer support.
		InitializeComponent()
		// TODO: Add constructor code after the InitializeComponent() call.

[STAThread]
def Main(argv as (string)):
	Application.EnableVisualStyles()
	Application.SetCompatibleTextRenderingDefault(false)
	Application.Run(MainForm())

