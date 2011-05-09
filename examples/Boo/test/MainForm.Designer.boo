namespace test

partial class MainForm(System.Windows.Forms.Form):
	private components as System.ComponentModel.IContainer = null
	
	protected override def Dispose(disposing as bool):
		if disposing:
			if components is not null:
				components.Dispose()
		super(disposing)
	
	// This method is required for Windows Forms designer support.
	// Do not change the method contents inside the source code editor. The Forms designer might
	// not be able to load this method if it was changed manually.
	def InitializeComponent():
		self.measureButton = System.Windows.Forms.Button()
		self.resultLabel = System.Windows.Forms.Label()
		self.SuspendLayout()
		# 
		# measureButton
		# 
		self.measureButton.Dock = System.Windows.Forms.DockStyle.Left
		self.measureButton.Location = System.Drawing.Point(0, 0)
		self.measureButton.Name = "measureButton"
		self.measureButton.Size = System.Drawing.Size(75, 37)
		self.measureButton.TabIndex = 0
		self.measureButton.Text = "measure"
		self.measureButton.UseVisualStyleBackColor = true
		self.measureButton.Click += self.MeasureButtonClick as System.EventHandler
		# 
		# resultLabel
		# 
		self.resultLabel.Dock = System.Windows.Forms.DockStyle.Right
		self.resultLabel.Font = System.Drawing.Font("Microsoft Sans Serif", 12, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, cast(System.Byte,0))
		self.resultLabel.Location = System.Drawing.Point(81, 0)
		self.resultLabel.Name = "resultLabel"
		self.resultLabel.Size = System.Drawing.Size(211, 37)
		self.resultLabel.TabIndex = 1
		self.resultLabel.Text = "- - -"
		self.resultLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		# 
		# MainForm
		# 
		self.AutoScaleDimensions = System.Drawing.SizeF(6, 13)
		self.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		self.ClientSize = System.Drawing.Size(292, 37)
		self.Controls.Add(self.resultLabel)
		self.Controls.Add(self.measureButton)
		self.Name = "MainForm"
		self.Text = "test - Boo"
		self.ResumeLayout(false)
	private resultLabel as System.Windows.Forms.Label
	private measureButton as System.Windows.Forms.Button
	
	private def MeasureButtonClick(sender as object, e as System.EventArgs):
		pm as pm_context
		power as single
		if pm_init(pm) == 0:
			// open the probe with serial number 27
			if pm_open(pm, 27) == 0:
				// set the frequency to 1 GHz
				pm_set_frequency(pm, 1000000000)
				// set the average count to 1000
				pm_set_averages(pm, 1000)
				if pm_measure(pm, power) == 0: 
					resultLabel.Text = power.ToString() + " dBm"
				else:
					resultLabel.Text = "Measurement failed!"
				pm_close(pm)
			else:
				resultLabel.Text = "Open failed!"
		else:
			resultLabel.Text = "Init failed!"		
						
					
			
			
