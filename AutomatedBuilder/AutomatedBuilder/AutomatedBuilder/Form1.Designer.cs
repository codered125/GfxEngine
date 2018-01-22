namespace AutomatedBuilder
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.amtInputBox = new System.Windows.Forms.TextBox();
            this.amtLabel = new System.Windows.Forms.Label();
            this.commadletLabel = new System.Windows.Forms.Label();
            this.commandletTextBox = new System.Windows.Forms.TextBox();
            this.EngineDropDown = new System.Windows.Forms.ComboBox();
            this.mapInputBox = new System.Windows.Forms.TextBox();
            this.MapDisplay = new System.Windows.Forms.CheckedListBox();
            this.MapLabel = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.ProjectURL = new System.Windows.Forms.Label();
            this.ProjectBox = new System.Windows.Forms.TextBox();
            this.LightTitle = new System.Windows.Forms.Label();
            this.PAckageTitle = new System.Windows.Forms.Label();
            this.amtBrowse = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.ue4Browse = new System.Windows.Forms.Button();
            this.UProjectBrowse = new System.Windows.Forms.Button();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.MapTitle = new System.Windows.Forms.Label();
            this.OutputLoc = new System.Windows.Forms.TextBox();
            this.EngineTitle = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // amtInputBox
            // 
            this.amtInputBox.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.amtInputBox.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.amtInputBox.ForeColor = System.Drawing.Color.White;
            this.amtInputBox.Location = new System.Drawing.Point(67, 67);
            this.amtInputBox.Name = "amtInputBox";
            this.amtInputBox.Size = new System.Drawing.Size(530, 25);
            this.amtInputBox.TabIndex = 0;
            this.amtInputBox.Text = "D:\\Epic\\UE_4.16\\Engine\\Binaries\\DotNET\\AutomationTool";
            this.amtInputBox.TextChanged += new System.EventHandler(this.automationToolTexboxChange);
            this.amtInputBox.Leave += new System.EventHandler(this.automationTextUnFocus);
            // 
            // amtLabel
            // 
            this.amtLabel.AutoSize = true;
            this.amtLabel.BackColor = System.Drawing.Color.Transparent;
            this.amtLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.amtLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.amtLabel.ForeColor = System.Drawing.Color.Snow;
            this.amtLabel.Location = new System.Drawing.Point(64, 32);
            this.amtLabel.Name = "amtLabel";
            this.amtLabel.Size = new System.Drawing.Size(338, 21);
            this.amtLabel.TabIndex = 1;
            this.amtLabel.Text = "Locate your Autiomation tool and paste the link";
            // 
            // commadletLabel
            // 
            this.commadletLabel.AutoSize = true;
            this.commadletLabel.BackColor = System.Drawing.Color.Transparent;
            this.commadletLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.commadletLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.commadletLabel.ForeColor = System.Drawing.Color.Snow;
            this.commadletLabel.Location = new System.Drawing.Point(71, 134);
            this.commadletLabel.Name = "commadletLabel";
            this.commadletLabel.Size = new System.Drawing.Size(319, 21);
            this.commadletLabel.TabIndex = 3;
            this.commadletLabel.Text = "Locate your UE4Editor.Exe and paste the link.";
            // 
            // commandletTextBox
            // 
            this.commandletTextBox.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.commandletTextBox.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.commandletTextBox.ForeColor = System.Drawing.Color.White;
            this.commandletTextBox.Location = new System.Drawing.Point(67, 167);
            this.commandletTextBox.Name = "commandletTextBox";
            this.commandletTextBox.Size = new System.Drawing.Size(530, 25);
            this.commandletTextBox.TabIndex = 2;
            this.commandletTextBox.Text = "D:\\Epic\\UE_4.16\\Engine\\Binaries\\Win64\\UE4Editor.exe";
            this.commandletTextBox.TextChanged += new System.EventHandler(this.commandletTexboxChange);
            this.commandletTextBox.Leave += new System.EventHandler(this.commandletTextUnFocus);
            // 
            // EngineDropDown
            // 
            this.EngineDropDown.AllowDrop = true;
            this.EngineDropDown.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.EngineDropDown.FormattingEnabled = true;
            this.EngineDropDown.Items.AddRange(new object[] {
            "Unity",
            "Unreal Engine",
            "None"});
            this.EngineDropDown.Location = new System.Drawing.Point(805, 227);
            this.EngineDropDown.MaxDropDownItems = 3;
            this.EngineDropDown.Name = "EngineDropDown";
            this.EngineDropDown.Size = new System.Drawing.Size(101, 21);
            this.EngineDropDown.TabIndex = 4;
            this.EngineDropDown.SelectedIndexChanged += new System.EventHandler(this.EngineDropDown_SelectedIndexChanged);
            // 
            // mapInputBox
            // 
            this.mapInputBox.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.mapInputBox.ForeColor = System.Drawing.Color.White;
            this.mapInputBox.Location = new System.Drawing.Point(67, 287);
            this.mapInputBox.Name = "mapInputBox";
            this.mapInputBox.Size = new System.Drawing.Size(182, 22);
            this.mapInputBox.TabIndex = 5;
            this.mapInputBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MapEnter);
            // 
            // MapDisplay
            // 
            this.MapDisplay.BackColor = System.Drawing.Color.DimGray;
            this.MapDisplay.ForeColor = System.Drawing.SystemColors.Control;
            this.MapDisplay.FormattingEnabled = true;
            this.MapDisplay.Location = new System.Drawing.Point(390, 285);
            this.MapDisplay.Name = "MapDisplay";
            this.MapDisplay.Size = new System.Drawing.Size(302, 106);
            this.MapDisplay.TabIndex = 6;
            // 
            // MapLabel
            // 
            this.MapLabel.AutoSize = true;
            this.MapLabel.BackColor = System.Drawing.Color.Transparent;
            this.MapLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.MapLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MapLabel.ForeColor = System.Drawing.Color.Snow;
            this.MapLabel.Location = new System.Drawing.Point(64, 261);
            this.MapLabel.Name = "MapLabel";
            this.MapLabel.Size = new System.Drawing.Size(343, 21);
            this.MapLabel.TabIndex = 7;
            this.MapLabel.Text = "Type in your map name and select them to build";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.DodgerBlue;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Location = new System.Drawing.Point(755, 486);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(264, 41);
            this.button1.TabIndex = 8;
            this.button1.Text = "Build All Selected Maps";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // ProjectURL
            // 
            this.ProjectURL.AutoSize = true;
            this.ProjectURL.BackColor = System.Drawing.Color.Transparent;
            this.ProjectURL.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ProjectURL.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProjectURL.ForeColor = System.Drawing.Color.Snow;
            this.ProjectURL.Location = new System.Drawing.Point(64, 427);
            this.ProjectURL.Name = "ProjectURL";
            this.ProjectURL.Size = new System.Drawing.Size(275, 21);
            this.ProjectURL.TabIndex = 9;
            this.ProjectURL.Text = "Locate your Project  url and put it here";
            // 
            // ProjectBox
            // 
            this.ProjectBox.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.ProjectBox.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProjectBox.ForeColor = System.Drawing.Color.White;
            this.ProjectBox.Location = new System.Drawing.Point(67, 452);
            this.ProjectBox.Name = "ProjectBox";
            this.ProjectBox.Size = new System.Drawing.Size(530, 25);
            this.ProjectBox.TabIndex = 10;
            this.ProjectBox.Text = "D:\\Moman\\UnrealEngineProjects\\UAT\\UAT.uproject";
            this.ProjectBox.TextChanged += new System.EventHandler(this.ProjecturlTexboxChange);
            this.ProjectBox.Leave += new System.EventHandler(this.ProjecturlTexboxUnfocus);
            // 
            // LightTitle
            // 
            this.LightTitle.AutoSize = true;
            this.LightTitle.BackColor = System.Drawing.Color.Transparent;
            this.LightTitle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.LightTitle.Font = new System.Drawing.Font("Segoe UI Black", 15.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LightTitle.ForeColor = System.Drawing.Color.Snow;
            this.LightTitle.Location = new System.Drawing.Point(460, 109);
            this.LightTitle.Name = "LightTitle";
            this.LightTitle.Size = new System.Drawing.Size(156, 30);
            this.LightTitle.TabIndex = 11;
            this.LightTitle.Text = "Light Building";
            // 
            // PAckageTitle
            // 
            this.PAckageTitle.AutoSize = true;
            this.PAckageTitle.BackColor = System.Drawing.Color.Transparent;
            this.PAckageTitle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.PAckageTitle.Font = new System.Drawing.Font("Segoe UI Black", 15.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PAckageTitle.ForeColor = System.Drawing.Color.Snow;
            this.PAckageTitle.Location = new System.Drawing.Point(460, 9);
            this.PAckageTitle.Name = "PAckageTitle";
            this.PAckageTitle.Size = new System.Drawing.Size(406, 30);
            this.PAckageTitle.TabIndex = 12;
            this.PAckageTitle.Text = "Packaging (Currently in Development)";
            // 
            // amtBrowse
            // 
            this.amtBrowse.BackColor = System.Drawing.Color.DodgerBlue;
            this.amtBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.amtBrowse.Location = new System.Drawing.Point(617, 67);
            this.amtBrowse.Name = "amtBrowse";
            this.amtBrowse.Size = new System.Drawing.Size(75, 25);
            this.amtBrowse.TabIndex = 13;
            this.amtBrowse.Tag = "AutomationTool.exe";
            this.amtBrowse.Text = "Browse";
            this.amtBrowse.UseVisualStyleBackColor = false;
            this.amtBrowse.Click += new System.EventHandler(this.button2_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // ue4Browse
            // 
            this.ue4Browse.BackColor = System.Drawing.Color.DodgerBlue;
            this.ue4Browse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ue4Browse.Location = new System.Drawing.Point(617, 167);
            this.ue4Browse.Name = "ue4Browse";
            this.ue4Browse.Size = new System.Drawing.Size(75, 25);
            this.ue4Browse.TabIndex = 14;
            this.ue4Browse.Tag = "UE4Editor.exe";
            this.ue4Browse.Text = "Browse";
            this.ue4Browse.UseVisualStyleBackColor = false;
            this.ue4Browse.Click += new System.EventHandler(this.button2_Click);
            // 
            // UProjectBrowse
            // 
            this.UProjectBrowse.BackColor = System.Drawing.Color.DodgerBlue;
            this.UProjectBrowse.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.UProjectBrowse.Location = new System.Drawing.Point(617, 452);
            this.UProjectBrowse.Name = "UProjectBrowse";
            this.UProjectBrowse.Size = new System.Drawing.Size(75, 25);
            this.UProjectBrowse.TabIndex = 15;
            this.UProjectBrowse.Tag = ".uproject";
            this.UProjectBrowse.Text = "Browse";
            this.UProjectBrowse.UseVisualStyleBackColor = false;
            this.UProjectBrowse.Click += new System.EventHandler(this.button2_Click);
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "img2.jpeg");
            this.imageList1.Images.SetKeyName(1, "BlackandGrey.jpg");
            // 
            // MapTitle
            // 
            this.MapTitle.AutoSize = true;
            this.MapTitle.BackColor = System.Drawing.Color.Transparent;
            this.MapTitle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.MapTitle.Font = new System.Drawing.Font("Segoe UI Black", 15.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MapTitle.ForeColor = System.Drawing.Color.Snow;
            this.MapTitle.Location = new System.Drawing.Point(460, 227);
            this.MapTitle.Name = "MapTitle";
            this.MapTitle.Size = new System.Drawing.Size(153, 30);
            this.MapTitle.TabIndex = 16;
            this.MapTitle.Text = "Maps / Levels";
            // 
            // OutputLoc
            // 
            this.OutputLoc.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.OutputLoc.ForeColor = System.Drawing.SystemColors.Control;
            this.OutputLoc.Location = new System.Drawing.Point(805, 285);
            this.OutputLoc.Name = "OutputLoc";
            this.OutputLoc.Size = new System.Drawing.Size(280, 22);
            this.OutputLoc.TabIndex = 17;
            this.OutputLoc.Text = "Package Location";
            this.OutputLoc.TextChanged += new System.EventHandler(this.OutputLoc_TextChanged);
            // 
            // EngineTitle
            // 
            this.EngineTitle.AutoSize = true;
            this.EngineTitle.BackColor = System.Drawing.Color.Transparent;
            this.EngineTitle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.EngineTitle.Font = new System.Drawing.Font("Segoe UI Black", 15.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EngineTitle.ForeColor = System.Drawing.Color.Snow;
            this.EngineTitle.Location = new System.Drawing.Point(800, 162);
            this.EngineTitle.Name = "EngineTitle";
            this.EngineTitle.Size = new System.Drawing.Size(269, 30);
            this.EngineTitle.TabIndex = 18;
            this.EngineTitle.Text = "Engine / Package Output";
            // 
            // Form1
            // 
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(1097, 555);
            this.Controls.Add(this.EngineTitle);
            this.Controls.Add(this.OutputLoc);
            this.Controls.Add(this.MapTitle);
            this.Controls.Add(this.UProjectBrowse);
            this.Controls.Add(this.ue4Browse);
            this.Controls.Add(this.amtBrowse);
            this.Controls.Add(this.PAckageTitle);
            this.Controls.Add(this.LightTitle);
            this.Controls.Add(this.ProjectBox);
            this.Controls.Add(this.ProjectURL);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.MapLabel);
            this.Controls.Add(this.MapDisplay);
            this.Controls.Add(this.mapInputBox);
            this.Controls.Add(this.EngineDropDown);
            this.Controls.Add(this.commadletLabel);
            this.Controls.Add(this.commandletTextBox);
            this.Controls.Add(this.amtLabel);
            this.Controls.Add(this.amtInputBox);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.SystemColors.Control;
            this.Name = "Form1";
        //    this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox amtInputBox;
        private System.Windows.Forms.Label amtLabel;
        private System.Windows.Forms.Label commadletLabel;
        private System.Windows.Forms.TextBox commandletTextBox;
        private System.Windows.Forms.ComboBox EngineDropDown;
        private System.Windows.Forms.TextBox mapInputBox;
        private System.Windows.Forms.CheckedListBox MapDisplay;
        private System.Windows.Forms.Label MapLabel;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label ProjectURL;
        private System.Windows.Forms.TextBox ProjectBox;
        private System.Windows.Forms.Label LightTitle;
        private System.Windows.Forms.Label PAckageTitle;
        private System.Windows.Forms.Button amtBrowse;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button ue4Browse;
        private System.Windows.Forms.Button UProjectBrowse;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Label MapTitle;
        private System.Windows.Forms.TextBox OutputLoc;
        private System.Windows.Forms.Label EngineTitle;
    }
}

