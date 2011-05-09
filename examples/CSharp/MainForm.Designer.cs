/*
 * Erstellt mit SharpDevelop.
 * Benutzer: AHaupt
 * Datum: 05.03.2009
 * Zeit: 16:27
 * 
 * Sie können diese Vorlage unter Extras > Optionen > Codeerstellung > Standardheader ändern.
 */

using PM600x;
 
namespace test
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.measureButton = new System.Windows.Forms.Button();
			this.resultLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// measureButton
			// 
			this.measureButton.Dock = System.Windows.Forms.DockStyle.Left;
			this.measureButton.Location = new System.Drawing.Point(0, 0);
			this.measureButton.Name = "measureButton";
			this.measureButton.Size = new System.Drawing.Size(163, 44);
			this.measureButton.TabIndex = 0;
			this.measureButton.Text = "measure";
			this.measureButton.UseVisualStyleBackColor = true;
			this.measureButton.Click += new System.EventHandler(this.Button1Click);
			// 
			// resultLabel
			// 
			this.resultLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.resultLabel.Dock = System.Windows.Forms.DockStyle.Right;
			this.resultLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.resultLabel.Location = new System.Drawing.Point(169, 0);
			this.resultLabel.Name = "resultLabel";
			this.resultLabel.Size = new System.Drawing.Size(412, 44);
			this.resultLabel.TabIndex = 1;
			this.resultLabel.Text = "- - -";
			this.resultLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.ClientSize = new System.Drawing.Size(581, 44);
			this.Controls.Add(this.resultLabel);
			this.Controls.Add(this.measureButton);
			this.Name = "MainForm";
			this.Text = "test - C#";
			this.ResumeLayout(false);
		}
		private System.Windows.Forms.Button measureButton;
		private System.Windows.Forms.Label resultLabel;
		
		unsafe void Button1Click(object sender, System.EventArgs e)
		{
			float power;			
			PM600x.PM600x_Declarations.pm_context pm;
			
			if (PM600x.PM600x_Declarations.pm_init(out pm) == 0) {
				// open the probe with serial number 27 
				// you have to change this to the serial number of your probe
				if (PM600x.PM600x_Declarations.pm_open(out pm, 27) == 0) {
					// set the frequency to 1 GHz
					PM600x.PM600x_Declarations.pm_set_frequency(out pm, 1000000000);
					// set the average count to 1000
					PM600x.PM600x_Declarations.pm_set_averages(out pm, 1000);
					// measure the power
					if (PM600x.PM600x_Declarations.pm_measure(out pm, out power) == 0)						
						resultLabel.Text = power.ToString() + " dBm";
					else
						resultLabel.Text = "Measurement failed!";
					// close the probe
					PM600x.PM600x_Declarations.pm_close(out pm);				
				}
				else
					resultLabel.Text = "Open failed!";
			}
			else
				resultLabel.Text = "Init failed!";
		}
	}
}
