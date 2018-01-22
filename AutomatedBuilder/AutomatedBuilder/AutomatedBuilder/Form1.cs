using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace AutomatedBuilder
{
    public partial class Form1 : Form
    {
        public enum EngineType { None, Unity, UE4 }
        public class EngineDetails
        {
            public string AT, CT, PURL, Output;
            public EngineType type;
            public List<string> MapListsMapL;

            public EngineDetails(string inURL,string inOutput ,string inAT, string inCT, EngineType inType, List<string> inList)
            {
                PURL = inURL;
                AT = inAT;
                CT = inCT;
                type = inType;
                MapListsMapL = inList;
                Output = inOutput;
            }

        }
        string AutomationText, CommandletText, ProjectText, outputLocText = "";
        EngineType selectedType;
        public List<string> MapLists = new List<string>();

        public Form1()
        {
            InitializeComponent();
            AutomationText = amtInputBox.Text;
            CommandletText = commandletTextBox.Text;
            ProjectText = ProjectBox.Text;

        }

        private void automationToolTexboxChange(object sender, EventArgs e)
        {
            AutomationText = amtInputBox.Text;
        }

        private void automationTextUnFocus(object sender, EventArgs e)
        {
            amtLabel.Text = amtLabel.Text + " Changed";
        }

        private void commandletTexboxChange(object sender, EventArgs e)
        {
            CommandletText = commandletTextBox.Text;
        }

        private void commandletTextUnFocus(object sender, EventArgs e)
        {
            commadletLabel.Text = commadletLabel.Text + " Changed"; 
        }

        private void ProjecturlTexboxChange(object sender, EventArgs e)
        {
            ProjectText = ProjectBox.Text;
        }

        private void ProjecturlTexboxUnfocus(object sender, EventArgs e)
        {
            ProjectURL.Text = ProjectURL.Text + " Changed"; ;
        }


        private void MapEnter(object sender, KeyEventArgs e)
        {
            if (e.KeyData != Keys.Enter) return;
            string textInstance = mapInputBox.Text.ToUpper();
            bool matched = false; ;
            MapLists.ForEach(m => { if (m.ToUpper() == textInstance) matched = true; });
            if (!matched)
            {
                MapLists.Add(textInstance);
                MapDisplay.Items.Add(textInstance);
            }
        }



        private void EngineDropDown_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedType = EngineType.None;
            switch (EngineDropDown.SelectedItem as string)
            {
                case "Unity":
                    selectedType = EngineType.Unity;
                    break;

                case "Unreal Engine":
                    selectedType = EngineType.UE4;
                    break;

                default:
                    selectedType = EngineType.None;
                    break;
            }
        }

        private void OutputLoc_TextChanged(object sender, EventArgs e)
        {
            outputLocText = OutputLoc.Text;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            List<string> MapListsFilterd = new List<string>();
            MapLists.ForEach(m =>
            {
                if (MapDisplay.CheckedItems.Contains(m)) MapListsFilterd.Add(m);
            });
            Backend.ComposeBatFile(new EngineDetails(ProjectText, outputLocText,AutomationText, CommandletText, selectedType, MapListsFilterd));
        }



        private void button2_Click(object sender, EventArgs e)
        {
            int size = -1;
            DialogResult result =  openFileDialog1.ShowDialog(); // Show the dialog.
            if (result == DialogResult.OK) // Test result.
            {
                string file = openFileDialog1.FileName;
                try
                {
                    string text = File.ReadAllText(file);
                    size = text.Length;
                    Button buttonSender = (Button)sender;
                    if (file.Contains(buttonSender.Tag.ToString()))
                    {
                        switch(buttonSender.Tag.ToString())
                        {
                            case "AutomationTool.exe":
                                AutomationText = file;
                                amtInputBox.Text = file;
                                break;

                            case "UE4Editor.exe":
                                CommandletText = file;
                                commandletTextBox.Text = file;
                                break;

                            case ".uproject":
                                ProjectText = file;
                                ProjectBox.Text = file;
                                break;
;
                        }

                        Console.WriteLine(file);
                    }
                }
                catch (IOException)
                {
                }
            }
        }
    }
}
