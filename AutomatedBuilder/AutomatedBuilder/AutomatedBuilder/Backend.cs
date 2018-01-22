using System;
using System.Diagnostics;


namespace AutomatedBuilder
{
    public class Backend
    {
        public static void ComposeBatFile(Form1.EngineDetails inDetils)
        {
            string batFilePath = System.IO.Directory.GetCurrentDirectory()+ "\\lightBuild.bat";

            if (!System.IO.File.Exists(batFilePath))
            {
                using (System.IO.FileStream fs = System.IO.File.Create(batFilePath))
                {
                    fs.Close();
                }
            }

            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(batFilePath))
            {
                string maps = "-maps=";
                inDetils.MapListsMapL.ForEach(m => maps += "+" + m);
                sw.WriteLine(inDetils.CT + " " + inDetils.PURL + " -run=resavepackages -buildlighting -allowcommandletrendering " + maps);
            }

            Process procc = null;
            try
            {
                procc = new Process();
                procc.StartInfo.WorkingDirectory = batFilePath;
                procc.StartInfo.FileName = batFilePath;
                procc.StartInfo.CreateNoWindow = false;
                procc.Start();
                procc.WaitForExit();
                Console.WriteLine("dun now");
                Package(inDetils);
            }
            catch (Exception e)
            {

            }
        }



        public static void Package(Form1.EngineDetails inDetails)
        {
            string batFilePath = System.IO.Directory.GetCurrentDirectory() + "\\package.bat";

            if (!System.IO.File.Exists(batFilePath))
            {
                using (System.IO.FileStream fs = System.IO.File.Create(batFilePath))
                {
                    fs.Close();
                }
            }

            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(batFilePath))
            {
                string maps = "-maps=";
                inDetails.MapListsMapL.ForEach(m => maps += "+" + m);
                sw.WriteLine(inDetails.AT + " BuildCookRun -project="+  inDetails.PURL + " -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook " + maps + "-build -stage -pak -archive -archivedirectory="+inDetails.Output);
            }

            Process procc = null;
            try
            {
                procc = new Process();
                procc.StartInfo.WorkingDirectory = batFilePath;
                procc.StartInfo.FileName = batFilePath;
                procc.StartInfo.CreateNoWindow = false;
                procc.Start();
                procc.WaitForExit();
                Console.WriteLine("dun now");
            }
            catch (Exception e)
            {

            }
        }
    }
}