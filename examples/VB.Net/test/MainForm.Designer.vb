'
' Erstellt mit SharpDevelop.
' Benutzer: AHaupt
' Datum: 06.03.2009
' Zeit: 11:34
' 
' Sie können diese Vorlage unter Extras > Optionen > Codeerstellung > Standardheader ändern.
'

Partial Class MainForm
	Inherits System.Windows.Forms.Form
	
	''' <summary>
	''' Designer variable used to keep track of non-visual components.
	''' </summary>
	Private components As System.ComponentModel.IContainer
	
	''' <summary>
	''' Disposes resources used by the form.
	''' </summary>
	''' <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
	Protected Overrides Sub Dispose(ByVal disposing As Boolean)
		If disposing Then
			If components IsNot Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(disposing)
	End Sub
	
	''' <summary>
	''' This method is required for Windows Forms designer support.
	''' Do not change the method contents inside the source code editor. The Forms designer might
	''' not be able to load this method if it was changed manually.
	''' </summary>
	Private Sub InitializeComponent()
		Me.measureButton = New System.Windows.Forms.Button
		Me.resultLabel = New System.Windows.Forms.Label
		Me.SuspendLayout
		'
		'measureButton
		'
		Me.measureButton.Dock = System.Windows.Forms.DockStyle.Left
		Me.measureButton.Location = New System.Drawing.Point(0, 0)
		Me.measureButton.Name = "measureButton"
		Me.measureButton.Size = New System.Drawing.Size(75, 43)
		Me.measureButton.TabIndex = 0
		Me.measureButton.Text = "measure"
		Me.measureButton.UseVisualStyleBackColor = true
		AddHandler Me.measureButton.Click, AddressOf Me.MeasureButtonClick
		'
		'resultLabel
		'
		Me.resultLabel.Dock = System.Windows.Forms.DockStyle.Right
		Me.resultLabel.Font = New System.Drawing.Font("Microsoft Sans Serif", 12!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0,Byte))
		Me.resultLabel.Location = New System.Drawing.Point(81, 0)
		Me.resultLabel.Name = "resultLabel"
		Me.resultLabel.Size = New System.Drawing.Size(211, 43)
		Me.resultLabel.TabIndex = 1
		Me.resultLabel.Text = "- - -"
		Me.resultLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		'
		'MainForm
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6!, 13!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(292, 43)
		Me.Controls.Add(Me.resultLabel)
		Me.Controls.Add(Me.measureButton)
		Me.Name = "MainForm"
		Me.Text = "test - VB.Net"
		Me.ResumeLayout(false)
	End Sub
	Private resultLabel As System.Windows.Forms.Label
	Private measureButton As System.Windows.Forms.Button
	
	Sub MeasureButtonClick(sender As Object, e As EventArgs)
		Dim pm As PM6006.pm_context
		Dim power As Single
		
		If PM6006.pm_init(pm) = 0 Then
			' open the probe with serial number 27.
			' you have to change this to your probes serial
			If PM6006.pm_open(pm, 27) = 0 Then
				' set the frequency to 1 GHz
				PM6006.pm_set_frequency(pm, 1000000000)
				' set the average count to 1000
				PM6006.pm_set_averages(pm, 1000)
				' measure the power
				If PM6006.pm_measure(pm, power) = 0 Then
					resultLabel.Text = power.ToString() + " dBm"
				Else
					resultLabel.Text = "Measurement failed!"
				End If
				PM6006.pm_close(pm)
			Else
				resultLabel.Text = "Open failed!"
			End If
		Else
			resultLabel.Text = "Init failed!"
		End If		
	End Sub
End Class
