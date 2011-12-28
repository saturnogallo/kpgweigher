using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
namespace RT9609CS
{
    /*
     * Script keyword
     * function funcname(type param1, type param2)
     * 
     */
    internal class Script
    {
        Stack<object> stack;
        Stack<int> ifcount_stack;
        Stack<int> wcount_stack;
        Stack<int> argcount_stack;
        Stack<int> while_mark;
        Stack<int> ptr_stack;
        List<string> scripts;
        Dictionary<string, object> global_var;
        Dictionary<string, object> local_var;
        Dictionary<string, int> func;
        public Script(string script_file)
        {
            stack = new Stack<object>();
            scripts = new List<string>();
            global_var = new Dictionary<string, object>();
            local_var = new Dictionary<string, object>();
            func = new Dictionary<string, int>();
            argcount_stack = new Stack<int>();
            ifcount_stack = new Stack<int>();
            wcount_stack = new Stack<int>();
            while_mark = new Stack<int>();
            ptr_stack = new Stack<int>();
            using (StreamReader sr = new StreamReader(script_file))
            {
                string line;
                bool infunc = false;
                while (!sr.EndOfStream)
                {
                    line = sr.ReadLine().Trim();
                    if (line.Trim().IndexOf("/") >= 0)
                        continue;

                    if (line.ToLower().IndexOf("function ") == 0)
                    {
                        if (infunc) //last function is not end with end function
                        {
                            scripts.Add("return");
                        }
                        scripts.Add(line);
                        string[] items = line.Remove(0, "function ".Length).Trim().Split(new char[] { '(', ')', ',' });
                        func[items[0]] = scripts.Count - 1;
                        infunc = true;
                        continue;
                    }
                    if (line.ToLower().IndexOf("end function") == 0)
                    {
                        if (infunc)
                        {
                            scripts.Add("return");
                            infunc = false;
                            continue;
                        }
                    }
                    scripts.Add(line);
                }
            }
        }
        public void ResetScript()
        {
            local_var.Clear();
            global_var.Clear();
            stack.Clear();
            script_ptr = 0;
            argcount_stack.Clear();
            ifcount_stack.Clear();
            while_mark.Clear();
        }
        private int script_ptr = 0;

        public void CallFunc(string funcname, params object[] args)
        {
            if (!func.ContainsKey(funcname))
                throw new Exception("Function " + funcname + " is not found");
            string[] items = scripts[func[funcname]].Trim().Split(new char[] { '(', ')', ',' });

            foreach (string key in local_var.Keys)
                stack.Push(local_var[key]);
            argcount_stack.Push(local_var.Count);

            local_var.Clear();
            ifcount_stack.Push(0);
            wcount_stack.Push(0);
            int index = 0;
            for (int p = 1; p < items.Length; p++)
            {
                string parm = items[p];
                if (parm.Trim().Length == 0)
                    continue;
                string[] parms = parm.Split(new char[] { ' ' });
                local_var[parms[1].Trim()] = args[index++];
            }
            ptr_stack.Push(script_ptr);
            script_ptr = func[funcname] + 1;
        }
        private bool HasKeyword(string line, string key)
        {
            return (line.Trim().ToLower().IndexOf(key) == 0);
        }
        private String ToStr(string l)
        {
            l = l.Trim();
            if (l.StartsWith("\""))
                return l.Trim(new char[] { '"' });
            if (local_var.ContainsKey(l))
                return (local_var[l] as String);
            if (global_var.ContainsKey(l))
                return (global_var[l] as String);
            return "";
        }
        private Int32 ToInt(string l)
        {
            l = l.Trim();
            if (l.IndexOfAny("-0123456789".ToCharArray()) == 0)
                return Int32.Parse(l);
            if (local_var.ContainsKey(l))
                return (Int32)local_var[l];
            if (global_var.ContainsKey(l))
                return (Int32)global_var[l];
            return 0;
        }
        private Double ToDouble(string l)
        {
            l = l.Trim();
            if (l.IndexOfAny("-0123456789.".ToCharArray()) == 0)
                return Double.Parse(l);
            if (local_var.ContainsKey(l))
                return (Double)local_var[l];
            if (global_var.ContainsKey(l))
                return (Double)global_var[l];
            return 0;
        }
        private bool checkcondition(string cond)
        {
            string l_op, r_op, op;
            cond = cond.Replace(" then", "");
            cond = cond.Trim(new char[] { '(', ')', ' ' });
            int pos = 0;
            op = "";
            if (cond.IndexOf("==") > 0)
            {
                op = "==";
                pos = cond.IndexOf(op);
            }
            if (cond.IndexOf("!=") > 0)
            {
                op = "!=";
                pos = cond.IndexOf(op);
            }
            if (cond.IndexOf(">=") > 0)
            {
                op = ">=";
                pos = cond.IndexOf(op);
            }
            if (cond.IndexOf("<=") > 0)
            {
                op = "<=";
                pos = cond.IndexOf(op);
            }
            if (cond.IndexOf("< ") > 0)
            {
                op = "<";
                pos = cond.IndexOf(op);
            }
            if (cond.IndexOf("> ") > 0)
            {
                op = ">";
                pos = cond.IndexOf(op);
            }
            if (op == "")
                throw new Exception("Incorrect condition string " + cond);

            l_op = cond.Substring(0, pos).Trim();
            r_op = cond.Substring(pos + 2, cond.Length - pos - 2).Trim();

            object lobj = null;
            if (local_var.ContainsKey(l_op))
            {
                lobj = local_var[l_op];
            }
            if (global_var.ContainsKey(l_op))
            {
                lobj = global_var[l_op];
            }

            if (lobj.GetType().ToString() == "System.Int32")
            {
                Int32 l = (Int32)lobj;
                Int32 r = ToInt(r_op);

                if (op == "==")
                    return l == r;
                if (op == "!=")
                    return l != r;
                if (op == ">=")
                    return l >= r;
                if (op == "<=")
                    return l <= r;
                if (op == ">")
                    return l > r;
                if (op == "<")
                    return l < r;
            }
            if (lobj.GetType().ToString() == "String")
            {
                String l = lobj as String;
                String r = ToStr(r_op);

                if (op == "==")
                    return l == r;
                if (op == "!=")
                    return l != r;
            }
            return (cond.Trim() == "1" || cond.Trim().ToLower() == "true");
        }
        public string VarType(string var)
        {
            if (local_var.ContainsKey(var))
                return local_var[var].GetType().ToString();
            if (global_var.ContainsKey(var))
                return global_var[var].GetType().ToString();
            return "";
        }
        public void SetVar(string var, object o)
        {
            if (local_var.ContainsKey(var))
                local_var[var] = o;
            if (global_var.ContainsKey(var))
                global_var[var] = o;

        }
        public string StepFunc()
        {
            string line = scripts[script_ptr++].TrimEnd(new char[] { ' ', ';' });
            if (HasKeyword(line, "if"))//compare
            {
                ifcount_stack.Push(ifcount_stack.Pop() + 1);
                if (!checkcondition(line.Remove(0, 2).Trim()))   //goto next else/endif at the same level
                {
                    int count = ifcount_stack.Peek();
                    while (true)
                    {
                        line = scripts[script_ptr++];
                        if (HasKeyword(line, "if"))
                            ifcount_stack.Push(ifcount_stack.Pop() + 1);
                        if (HasKeyword(line, "end if"))
                            ifcount_stack.Push(ifcount_stack.Pop() - 1);
                        if (HasKeyword(line, "else"))
                        {
                            if (ifcount_stack.Peek() == count)
                                break;
                        }
                        if (ifcount_stack.Peek() < count)
                            break;
                    }
                }
                return "";
            }
            if (HasKeyword(line, "else"))
            {
                //just jump to end if
                int count = ifcount_stack.Peek();
                while (true)
                {
                    line = scripts[script_ptr++];
                    if (HasKeyword(line, "if"))
                        ifcount_stack.Push(ifcount_stack.Pop() + 1);
                    if (HasKeyword(line, "end if"))
                        ifcount_stack.Push(ifcount_stack.Pop() - 1);
                    if (ifcount_stack.Peek() < count)
                        break;
                }
                return "";
            }
            if (HasKeyword(line, "end if"))
            {
                ifcount_stack.Push(ifcount_stack.Pop() - 1);
                return "";
            }

            if (HasKeyword(line, "while"))//compare
            {
                wcount_stack.Push(wcount_stack.Pop() + 1);
                while_mark.Push(script_ptr - 1);
                if (!checkcondition(line.Remove(0, 5).Trim()))   //goto next wend at the same level
                {
                    int count = wcount_stack.Peek();
                    while (true)
                    {
                        line = scripts[script_ptr++];
                        if (HasKeyword(line, "while"))
                            wcount_stack.Push(wcount_stack.Pop() + 1);
                        if (HasKeyword(line, "wend"))
                            wcount_stack.Push(wcount_stack.Pop() - 1);
                        if (wcount_stack.Peek() < count)
                            break;
                    }
                }
                return "";
            }
            if (HasKeyword(line, "wend"))
            {
                wcount_stack.Push(wcount_stack.Pop() - 1);
                script_ptr = while_mark.Peek();
                return "";
            }
            if (HasKeyword(line, "break")) //jump to wend directly
            {
                int count = wcount_stack.Peek();
                while (true)
                {
                    line = scripts[script_ptr++];
                    if (HasKeyword(line, "while"))
                        wcount_stack.Push(wcount_stack.Pop() + 1);
                    if (HasKeyword(line, "wend"))
                        wcount_stack.Push(wcount_stack.Pop() - 1);
                    if (wcount_stack.Peek() < count)
                        break;
                }
                return "";
            }
            if (HasKeyword(line, "return"))
            {
                wcount_stack.Pop();
                ifcount_stack.Pop();
                int argcount = argcount_stack.Pop();
                while (argcount-- > 0)
                {
                    stack.Pop();
                }

                if (argcount_stack.Count == 0)
                    return "exit";
                script_ptr = ptr_stack.Pop();
                return "";
            }
            if (HasKeyword(line, "call"))
            {
                line = line.Remove(0, 4).Trim();
                string[] items = line.Split(new char[] { '(', ')', ',' });
                object[] fp = new object[items.Length - 2];
                string[] citems = scripts[func[items[0]]].Trim().Split(new char[] { '(', ')', ',' });
                int i = 0;
                for (int p = 1; p < citems.Length; p++)
                {
                    string parm = citems[p];
                    if (parm.Trim().Length == 0)
                        continue;
                    string[] parms = parm.Split(new char[] { ' ' });

                    if (parms[0] == "string")
                        fp[i++] = items[p];
                    if (parms[0] == "double")
                        fp[i++] = Double.Parse(items[p]);
                    if (parms[0] == "int")
                        fp[i++] = Int32.Parse(items[p]);
                }
                CallFunc(items[0], fp);
                return "";
            }
            if (HasKeyword(line, "var"))
            {
                line = line.Remove(0, 4).Trim();
                string[] vars = line.Replace(" ", "").Split('=');
                if (vars[1] == "\"\"")
                    global_var[vars[0].Trim()] = vars[1].Trim(new char[] { '"' });
                if (vars[1] == "0")
                    global_var[vars[0].Trim()] = Int32.Parse(vars[1].Trim());
                if (vars[1] == "0.0")
                    global_var[vars[0].Trim()] = Double.Parse(vars[1].Trim());
                return "";
            }
            if (HasKeyword(line, "syscall"))
            {
                //todo
                return line.Remove(0, "syscall".Length).Trim();
            }
            string[] assigns = line.Replace(" ", "").Split('=');
            if (assigns.Length > 1)
            {
                if ((!local_var.ContainsKey(assigns[0])) && (!global_var.ContainsKey(assigns[0])))
                {
                    if (assigns[1] == "\"\"")
                        local_var[assigns[0].Trim()] = String.Empty;
                    if (assigns[1] == "0")
                        local_var[assigns[0].Trim()] = Int32.Parse(assigns[1].Trim());
                    if (assigns[1] == "0.0")
                        local_var[assigns[0].Trim()] = Double.Parse(assigns[1].Trim());
                    return "";
                }
                string op = VarType(assigns[0]);
                string[] ops = assigns[1].Replace(" ", "").Replace("+", " + ").Replace("-", " - ").Replace("/", " / ").Replace("*", " * ").Replace(".", " . ").Trim().Split(new char[] { ' ' });
                if (op == "System.String") //string case
                {
                    String a = ToStr(ops[0]);
                    int i = 1;
                    while (i < ops.Length)
                    {
                        if (ops[i] == ".")
                        {
                            a = a + ToStr(ops[i + 1]);
                            i = i + 2;
                        }
                    }
                    SetVar(assigns[0], a);
                }
                if (op == "System.Int32")
                {
                    Int32 a = ToInt(ops[0]);
                    int i = 1;
                    while (i < ops.Length)
                    {
                        Int32 b = ToInt(ops[i + 1]);
                        if (ops[i] == "+")
                        {
                            a = a + b;

                        }
                        if (ops[i] == "-")
                        {
                            a = a - b;

                        }
                        if (ops[i] == "*")
                        {
                            a = a * b;

                        }
                        if (ops[i] == "/")
                        {
                            a = a / b;

                        }
                        i = i + 2;
                    }
                    SetVar(assigns[0], a);
                }
                if (op == "System.Double")
                {
                    Double a = ToDouble(ops[0]);
                    int i = 1;
                    while (i < ops.Length)
                    {
                        Double b = ToDouble(ops[i + 1]);

                        if (ops[i] == "+")
                        {
                            a = a + b;

                        }
                        if (ops[i] == "-")
                        {
                            a = a - b;

                        }
                        if (ops[i] == "*")
                        {
                            a = a * b;

                        }
                        if (ops[i] == "/")
                        {
                            a = a / b;

                        }
                        i = i + 2;
                    }
                    SetVar(assigns[0], a);
                }
            }

            return "";
        }
    }
}
