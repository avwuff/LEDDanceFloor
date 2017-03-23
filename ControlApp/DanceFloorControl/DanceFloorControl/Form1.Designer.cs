namespace DanceFloorControl
{
    partial class frmControls
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.flowEffects = new System.Windows.Forms.FlowLayoutPanel();
            this.flowColors = new System.Windows.Forms.FlowLayoutPanel();
            this.flowBaseEffects = new System.Windows.Forms.FlowLayoutPanel();
            this.btnTapTempo = new System.Windows.Forms.Button();
            this.btnSendPresses = new System.Windows.Forms.Button();
            this.btnReboot = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.pageSetupDialog1 = new System.Windows.Forms.PageSetupDialog();
            this.tbBigness = new XComponent.SliderBar.MACTrackBar();
            this.tbAlpha = new XComponent.SliderBar.MACTrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.flowBaseColor = new System.Windows.Forms.FlowLayoutPanel();
            this.flowSecondColor = new System.Windows.Forms.FlowLayoutPanel();
            this.label7 = new System.Windows.Forms.Label();
            this.btnRelease = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.cbMidi = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // flowEffects
            // 
            this.flowEffects.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.flowEffects.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(224)))), ((int)(((byte)(192)))));
            this.flowEffects.Location = new System.Drawing.Point(692, 30);
            this.flowEffects.Margin = new System.Windows.Forms.Padding(0);
            this.flowEffects.Name = "flowEffects";
            this.flowEffects.Size = new System.Drawing.Size(647, 496);
            this.flowEffects.TabIndex = 0;
            // 
            // flowColors
            // 
            this.flowColors.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.flowColors.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.flowColors.Location = new System.Drawing.Point(753, 566);
            this.flowColors.Margin = new System.Windows.Forms.Padding(0);
            this.flowColors.Name = "flowColors";
            this.flowColors.Size = new System.Drawing.Size(586, 517);
            this.flowColors.TabIndex = 1;
            this.flowColors.Paint += new System.Windows.Forms.PaintEventHandler(this.flowColors_Paint);
            // 
            // flowBaseEffects
            // 
            this.flowBaseEffects.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(192)))));
            this.flowBaseEffects.Location = new System.Drawing.Point(16, 566);
            this.flowBaseEffects.Margin = new System.Windows.Forms.Padding(0);
            this.flowBaseEffects.Name = "flowBaseEffects";
            this.flowBaseEffects.Size = new System.Drawing.Size(720, 517);
            this.flowBaseEffects.TabIndex = 2;
            this.flowBaseEffects.Paint += new System.Windows.Forms.PaintEventHandler(this.flowBaseEffects_Paint);
            // 
            // btnTapTempo
            // 
            this.btnTapTempo.Location = new System.Drawing.Point(306, 11);
            this.btnTapTempo.Name = "btnTapTempo";
            this.btnTapTempo.Size = new System.Drawing.Size(213, 49);
            this.btnTapTempo.TabIndex = 3;
            this.btnTapTempo.Text = "Tap Tempo";
            this.btnTapTempo.UseVisualStyleBackColor = true;
            this.btnTapTempo.Click += new System.EventHandler(this.btnTapTempo_Click);
            this.btnTapTempo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.btnTapTempo_KeyDown);
            this.btnTapTempo.KeyUp += new System.Windows.Forms.KeyEventHandler(this.btnTapTempo_KeyUp);
            // 
            // btnSendPresses
            // 
            this.btnSendPresses.Location = new System.Drawing.Point(138, 11);
            this.btnSendPresses.Name = "btnSendPresses";
            this.btnSendPresses.Size = new System.Drawing.Size(162, 49);
            this.btnSendPresses.TabIndex = 4;
            this.btnSendPresses.Text = "Send Presses";
            this.btnSendPresses.UseVisualStyleBackColor = true;
            this.btnSendPresses.Click += new System.EventHandler(this.btnSendPresses_Click);
            // 
            // btnReboot
            // 
            this.btnReboot.Location = new System.Drawing.Point(12, 12);
            this.btnReboot.Name = "btnReboot";
            this.btnReboot.Size = new System.Drawing.Size(120, 48);
            this.btnReboot.TabIndex = 5;
            this.btnReboot.Text = "Reboot";
            this.btnReboot.UseVisualStyleBackColor = true;
            this.btnReboot.Click += new System.EventHandler(this.btnReboot_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 83);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 20);
            this.label1.TabIndex = 7;
            this.label1.Text = "Bigness";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 261);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 20);
            this.label2.TabIndex = 8;
            this.label2.Text = "Alpha";
            // 
            // tbBigness
            // 
            this.tbBigness.BackColor = System.Drawing.Color.Transparent;
            this.tbBigness.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.tbBigness.Font = new System.Drawing.Font("Verdana", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbBigness.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(123)))), ((int)(((byte)(125)))), ((int)(((byte)(123)))));
            this.tbBigness.IndentHeight = 6;
            this.tbBigness.LargeChange = 10;
            this.tbBigness.Location = new System.Drawing.Point(16, 106);
            this.tbBigness.Maximum = 100;
            this.tbBigness.Minimum = 0;
            this.tbBigness.Name = "tbBigness";
            this.tbBigness.Size = new System.Drawing.Size(653, 104);
            this.tbBigness.TabIndex = 10;
            this.tbBigness.TickColor = System.Drawing.Color.FromArgb(((int)(((byte)(148)))), ((int)(((byte)(146)))), ((int)(((byte)(148)))));
            this.tbBigness.TickFrequency = 10;
            this.tbBigness.TickHeight = 5;
            this.tbBigness.TrackerColor = System.Drawing.Color.FromArgb(((int)(((byte)(24)))), ((int)(((byte)(130)))), ((int)(((byte)(198)))));
            this.tbBigness.TrackerSize = new System.Drawing.Size(64, 64);
            this.tbBigness.TrackLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.tbBigness.TrackLineHeight = 3;
            this.tbBigness.TrackLineSelectedColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.tbBigness.Value = 100;
            this.tbBigness.ValueChanged += new XComponent.SliderBar.ValueChangedHandler(this.tbBigness_ValueChanged);
            // 
            // tbAlpha
            // 
            this.tbAlpha.BackColor = System.Drawing.Color.Transparent;
            this.tbAlpha.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.tbAlpha.Font = new System.Drawing.Font("Verdana", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbAlpha.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(123)))), ((int)(((byte)(125)))), ((int)(((byte)(123)))));
            this.tbAlpha.IndentHeight = 6;
            this.tbAlpha.LargeChange = 10;
            this.tbAlpha.Location = new System.Drawing.Point(12, 295);
            this.tbAlpha.Maximum = 100;
            this.tbAlpha.Minimum = 0;
            this.tbAlpha.Name = "tbAlpha";
            this.tbAlpha.Size = new System.Drawing.Size(653, 104);
            this.tbAlpha.TabIndex = 11;
            this.tbAlpha.TickColor = System.Drawing.Color.FromArgb(((int)(((byte)(148)))), ((int)(((byte)(146)))), ((int)(((byte)(148)))));
            this.tbAlpha.TickFrequency = 10;
            this.tbAlpha.TickHeight = 5;
            this.tbAlpha.TrackerColor = System.Drawing.Color.FromArgb(((int)(((byte)(24)))), ((int)(((byte)(130)))), ((int)(((byte)(198)))));
            this.tbAlpha.TrackerSize = new System.Drawing.Size(64, 64);
            this.tbAlpha.TrackLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.tbAlpha.TrackLineHeight = 3;
            this.tbAlpha.TrackLineSelectedColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.tbAlpha.Value = 100;
            this.tbAlpha.ValueChanged += new XComponent.SliderBar.ValueChangedHandler(this.tbAlpha_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(688, 7);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 20);
            this.label3.TabIndex = 12;
            this.label3.Text = "Step Effect";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(749, 543);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(131, 20);
            this.label4.TabIndex = 13;
            this.label4.Text = "Step Effect Color";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 543);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(93, 20);
            this.label5.TabIndex = 14;
            this.label5.Text = "Base Effect";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 1095);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(134, 20);
            this.label6.TabIndex = 16;
            this.label6.Text = "Base Effect Color";
            // 
            // flowBaseColor
            // 
            this.flowBaseColor.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.flowBaseColor.Location = new System.Drawing.Point(16, 1118);
            this.flowBaseColor.Margin = new System.Windows.Forms.Padding(0);
            this.flowBaseColor.Name = "flowBaseColor";
            this.flowBaseColor.Size = new System.Drawing.Size(720, 460);
            this.flowBaseColor.TabIndex = 15;
            // 
            // flowSecondColor
            // 
            this.flowSecondColor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.flowSecondColor.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.flowSecondColor.Location = new System.Drawing.Point(753, 1118);
            this.flowSecondColor.Margin = new System.Windows.Forms.Padding(0);
            this.flowSecondColor.Name = "flowSecondColor";
            this.flowSecondColor.Size = new System.Drawing.Size(586, 460);
            this.flowSecondColor.TabIndex = 17;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(749, 1095);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(105, 20);
            this.label7.TabIndex = 18;
            this.label7.Text = "Second Color";
            // 
            // btnRelease
            // 
            this.btnRelease.Location = new System.Drawing.Point(525, 12);
            this.btnRelease.Name = "btnRelease";
            this.btnRelease.Size = new System.Drawing.Size(157, 48);
            this.btnRelease.TabIndex = 19;
            this.btnRelease.Text = "Release all Buttons";
            this.btnRelease.UseVisualStyleBackColor = true;
            this.btnRelease.Click += new System.EventHandler(this.btnRelease_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(16, 434);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(642, 92);
            this.button1.TabIndex = 20;
            this.button1.Text = "STROBE";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            this.button1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button1_MouseDown);
            // 
            // cbMidi
            // 
            this.cbMidi.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbMidi.FormattingEnabled = true;
            this.cbMidi.Location = new System.Drawing.Point(1171, 7);
            this.cbMidi.Name = "cbMidi";
            this.cbMidi.Size = new System.Drawing.Size(334, 28);
            this.cbMidi.TabIndex = 21;
            this.cbMidi.SelectedIndexChanged += new System.EventHandler(this.cbMidi_SelectedIndexChanged);
            // 
            // frmControls
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1351, 1545);
            this.Controls.Add(this.cbMidi);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnRelease);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.flowSecondColor);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.flowBaseColor);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbAlpha);
            this.Controls.Add(this.tbBigness);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnReboot);
            this.Controls.Add(this.btnSendPresses);
            this.Controls.Add(this.btnTapTempo);
            this.Controls.Add(this.flowBaseEffects);
            this.Controls.Add(this.flowColors);
            this.Controls.Add(this.flowEffects);
            this.Name = "frmControls";
            this.Text = "Dance Floor Controls";
            this.Load += new System.EventHandler(this.frmControls_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.FlowLayoutPanel flowEffects;
        private System.Windows.Forms.FlowLayoutPanel flowColors;
        private System.Windows.Forms.FlowLayoutPanel flowBaseEffects;
        private System.Windows.Forms.Button btnTapTempo;
        private System.Windows.Forms.Button btnSendPresses;
        private System.Windows.Forms.Button btnReboot;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PageSetupDialog pageSetupDialog1;
        private XComponent.SliderBar.MACTrackBar tbBigness;
        private XComponent.SliderBar.MACTrackBar tbAlpha;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.FlowLayoutPanel flowBaseColor;
        private System.Windows.Forms.FlowLayoutPanel flowSecondColor;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btnRelease;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox cbMidi;
    }
}

