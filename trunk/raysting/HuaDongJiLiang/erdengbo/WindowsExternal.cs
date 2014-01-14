using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using Word = Microsoft.Office.Interop.Word;
namespace Jmbo
{
    internal class Utility
    {
        static string str_appdir{
            get
            {
                return Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf(System.IO.Path.DirectorySeparatorChar));
            }
        }
    }
    static internal class Logger
    {
        public static void CriticalFailure(string line)
        {
        }
        static public void TouchFile(string file)
        {
            if (!File.Exists(file))
            {
                try
                {
                    FileStream fsLog = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.Read);
                    StreamWriter sw = new StreamWriter(fsLog);
                    sw.Close();
                    fsLog.Close();
                }
                catch (System.Exception ex)
                {
                }
            }
        }
        public static void Log(string line)
        {
            string fn = Path.Combine(Util.basedir, "run.txt");
            try
            {
                if (line == "")
                {
                    if (File.Exists(fn))
                        File.Delete(fn);
                    TouchFile(fn);
                    return;
                }
                FileStream fsLog = new FileStream(fn, FileMode.Append, FileAccess.Write, FileShare.Read);
                using (StreamWriter sw = new StreamWriter(fsLog))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\t" + line);
                }
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
            }
        }
    }
    class DocTool
    {
        public DocTool(TestDocument datadoc)
        {
            _testdoc = datadoc;
        }
        private TestDocument _testdoc;
        private Word.ApplicationClass _WordApplication;
        private Word.Document _WordDoc;
        private object missing = System.Reflection.Missing.Value;
        private void WordOpen(string strFileName)
        {
            object fileName = strFileName;
            object readOnly = false;
            object isVisible = true;
            

            _WordDoc = _WordApplication.Documents.Open(ref fileName, ref missing, ref readOnly,
            ref missing, ref missing, ref missing, ref missing, ref missing, ref missing,
            ref missing, ref missing, ref isVisible, ref missing, ref missing, ref missing, ref missing);

            _WordDoc.Activate();
        }

        private void WordQuit()
        {
            object missing = System.Reflection.Missing.Value;
            _WordApplication.Application.Quit(ref missing, ref missing, ref missing);
        }
        private void WordSave(string strFileName)
        {
            object missing = System.Reflection.Missing.Value;
            object fileName = strFileName;

            _WordDoc.Save();

            //object saveChanges = Word.WdSaveOptions.wdSaveChanges;
            //_WordDoc.Close(ref saveChanges, ref missing, ref missing);
        }

        private void WordSaveAs(string strFileName)
        {
            object missing = System.Reflection.Missing.Value;
            object fileName = strFileName;

            _WordDoc.SaveAs(ref fileName, ref missing, ref missing, ref missing, ref missing, ref missing, ref missing,
            ref missing, ref missing, ref missing, ref missing, ref missing, ref missing, ref missing, ref missing, ref missing);

            //object saveChanges = Word.WdSaveOptions.wdSaveChanges;
            //_WordDoc.Close(ref saveChanges, ref missing, ref missing);
        }
        internal void PrepareWord(string template)
        {
            _WordApplication = new Word.ApplicationClass();
            WordOpen(template);
        }
        internal void SaveWord(string wfile)
        {
            object missing = System.Reflection.Missing.Value;
            //ReplaceMailMergeField(_fieldNames, _replaceElements);
            WordSave(wfile);
            //_WordDoc.PrintPreview();
            //WordPrintDoc();
            WordQuit();
        }
        internal bool GoToBookmark(string mbm)
        {
            foreach (Word.Bookmark bm in _WordDoc.Bookmarks)
            {
                if (bm.Name == mbm)
                {
                    bm.Select();
                    return true;
                }
            }
            return false;
        }
        private void ScanForSubscript()
        {
            /*
            string a= "";
            string b= "";
            string c= "";
            if(_WordApplication.Selection.Range.Characters.Count < 4)
                return;
            for (int i = 3; i < _WordApplication.Selection.Range.Characters.Count-1; i++)
            {
                string word = _WordApplication.Selection.Range.Characters[i - 2].Text + _WordApplication.Selection.Range.Characters[i - 1].Text + _WordApplication.Selection.Range.Characters[i].Text;
                word = word.ToLower();
                if ((word == "rtp") ||
                    (word == "war") ||
                    (word == "wal") ||
                    (word == "wsn") ||
                    (word == "wzn") ||
                    (word == "wga") ||
                    (word == "whg") ||
                    (word == "win"))
                {
                    _WordApplication.Selection.Range.Characters[i - 1].Font.Subscript = 1;
                    _WordApplication.Selection.Range.Characters[i].Font.Subscript = 1;
                    _WordApplication.Selection.Range.Characters[i + 1].Font.Subscript = 0;
                }
            }
             */
        }
        internal void FillInTableByBookMarks(string mbm, List<string> sentences)
        {
            if (!GoToBookmark(mbm))
                return;


            object moveUnit = Microsoft.Office.Interop.Word.WdUnits.wdCell;
            object moveCount = 1;
            object moveExtend = Microsoft.Office.Interop.Word.WdMovementType.wdMove;

            object nr = 1;
            
            int nrow = 1;
            int count = sentences.Count;
            foreach (string line in sentences)
            {
                string[] graph = line.Split(new char[] { '\t' });
                _WordApplication.Selection.TypeText(graph[0]);
                if(graph[0].Length > 3)
                {
                    _WordApplication.Selection.SelectCell();
                    ScanForSubscript();
                }
                _WordApplication.Selection.Move(ref moveUnit, ref moveCount);
                if(graph[1] == "")
                    _WordApplication.Selection.TypeText("/");
                else
                _WordApplication.Selection.TypeText(graph[1]);
                
                if(--count > 0)
                    _WordApplication.Selection.InsertRowsBelow(ref nr);

            }
        }
        internal void InsertSentenceByBookMarks(string mbm, string text)
        {
            object moveUnit = Microsoft.Office.Interop.Word.WdUnits.wdParagraph;
            if (GoToBookmark(mbm))
            {
                _WordApplication.Selection.TypeText(text);
                GoToBookmark(mbm);
                _WordApplication.Selection.Expand(ref moveUnit);
                ScanForSubscript();
            }
        }

        internal void DeleteTableByBookMarks(string mbm)
        {
            if (GoToBookmark(mbm))
            {
                _WordApplication.Selection.SelectRow();
                _WordApplication.Selection.Cut();
            }
        }
        internal void AddTableRowByBookMarks(string mbm, int newrows)
        {
            object nr = newrows;
            if (GoToBookmark(mbm))
            {
                _WordApplication.Selection.InsertRowsBelow(ref nr);
            }
        }
        internal void RemoveTableRowByBookMarks(ICollection rows)
        {
            foreach (string mbm in rows)
            {
                if (GoToBookmark(mbm))
                {
                    _WordApplication.Selection.SelectRow();
                    _WordApplication.Selection.Cut();
                }
            }
        }
        public bool SearchReplace(string strOldText, string strNewText)
        {
            object replaceAll = Word.WdReplace.wdReplaceAll;
            object missing = System.Reflection.Missing.Value;

            //首先清除任何现有的格式设置选项，然后设置搜索字符串 strOldText。  
            _WordApplication.Selection.Find.ClearFormatting();
            _WordApplication.Selection.Find.Text = strOldText;

            _WordApplication.Selection.Find.Replacement.ClearFormatting();
            _WordApplication.Selection.Find.Replacement.Text = strNewText;

            if (_WordApplication.Selection.Find.Execute(
            ref missing, ref missing,
            ref missing, ref missing, ref missing,
            ref missing, ref missing,
            ref missing, ref missing, ref missing,
            ref replaceAll, ref missing,
            ref missing, ref missing, ref missing))
            {
                return true;
            }
            return false;
        } 
        internal void FillInHeader(string wfile, IEnumerable<JProperty> props,string ibcid, Microsoft.Office.Interop.Word.WdSeekView view)
        {

            _WordApplication.ActiveWindow.View.SeekView = view;
            _WordApplication.Selection.WholeStory();
            foreach (JProperty jp in props)
            {
                if (jp.Name.EndsWith("zsbh"))
                {
                    string key = jp.Name.Replace("_"+ibcid+"_","_");
                    SearchReplace(String.Format("<<{0}>>",key ), _testdoc.StrValue(jp.Name));
                }
            }
            //FillInField(wfile, props);
            _WordApplication.ActiveWindow.View.SeekView = Microsoft.Office.Interop.Word.WdSeekView.wdSeekMainDocument;
            
        }
        internal void FillInField(string wfile, IEnumerable<JProperty> props,string pageid,string ibcid)
        {
            

            Word.MailMerge mailMerge;
            mailMerge = _WordDoc.MailMerge;

            
            // Try to find the field name.
            foreach (Word.MailMergeField f in mailMerge.Fields)
            {
                f.Select();
                string text = "/";
                foreach (JProperty jp in props)
                {
                    if (jp.Type == JTokenType.Array)
                        continue;
                    // Assuming the field code is: MERGEFIELD  "mailMergeFieldName"
                    if ((f.Code.Text.IndexOf("MERGEFIELD  " + jp.Name.Replace("_" + ibcid + "_", "_")) > -1) ||
                        (f.Code.Text.IndexOf("MERGEFIELD  " + jp.Name.Replace("_"+pageid+"_","_")) > -1) ||
                        (f.Code.Text.IndexOf("MERGEFIELD  " + jp.Name) > -1))//field is found
                    {
                        // Replace selected field with supplied value.
                        text = _testdoc.FormatValue(jp, false);
                        break;
                    }
                }
                _WordApplication.Selection.TypeText(text);
            }
        }
    }
    static class JObjTool
    {
         static internal bool ExportToFile(JObject jo, string exportfile)
        {
            //serialize and save to local file
            JsonSerializer serializer = new JsonSerializer();
            serializer.NullValueHandling = NullValueHandling.Ignore;
            try
            {
                using (StreamWriter sw = new StreamWriter(exportfile))
                {
                    using (JsonWriter writer = new JsonTextWriter(sw))
                    {
                        serializer.Serialize(writer, jo);
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Logger.Log("Export to DB failed:" + ex.Message);
            }
            return false;
        }
    }
}
