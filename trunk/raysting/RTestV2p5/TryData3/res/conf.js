function getconftblst()
{
	return document.confform.tbselect;
}
function addconflist()
{
	var lstctl = getconftblst();
	var lstdata="@@conflst@@";
	var lstarray = lstdata.split(',');//seperate by ','
	lstctl.length = lstarray.length-1;//ignore the last ,
	var index=0;
	for (index=0;index< lstctl.length;index++){
		lstctl[index].value=lstarray[index];
		lstctl[index].text=lstarray[index];
	}
	lstctl.value = "@@confsz@@";
	
}
function exitconfform()
{
	return true;
}
function loadconfform()
{
	var ptr = document.confform;
	addconflist();
	ptr.rstdlt1.value = "@@rstdlt1@@";ptr.alphalt1.value = "@@alphalt1@@";ptr.betalt1.value = "@@betalt1@@";
	ptr.rstdlt2.value = "@@rstdlt2@@";ptr.alphalt2.value = "@@alphalt2@@";ptr.betalt2.value = "@@betalt2@@";
	ptr.rstdlt3.value = "@@rstdlt3@@";ptr.alphalt3.value = "@@alphalt3@@";ptr.betalt3.value = "@@betalt3@@";	
	ptr.rstdlt4.value = "@@rstdlt4@@";ptr.alphalt4.value = "@@alphalt4@@";ptr.betalt4.value = "@@betalt4@@";
	ptr.rstdlt5.value = "@@rstdlt5@@";ptr.alphalt5.value = "@@alphalt5@@";ptr.betalt5.value = "@@betalt5@@";
	ptr.rstdlt6.value = "@@rstdlt6@@";ptr.alphalt6.value = "@@alphalt6@@";ptr.betalt6.value = "@@betalt6@@";
	ptr.rstdlt7.value = "@@rstdlt7@@";ptr.alphalt7.value = "@@alphalt7@@";ptr.betalt7.value = "@@betalt7@@";
	ptr.rstdlt8.value = "@@rstdlt8@@";ptr.alphalt8.value = "@@alphalt8@@";ptr.betalt8.value = "@@betalt8@@";
	ptr.rstdlt9.value = "@@rstdlt9@@";ptr.alphalt9.value = "@@alphalt9@@";ptr.betalt9.value = "@@betalt9@@";
        if("@@confsz@@" == "Default"){
		ptr.rstdlt1.disabled = true ;ptr.alphalt1.disabled = true;ptr.betalt1.disabled = true;
		ptr.rstdlt2.disabled = true ;ptr.alphalt2.disabled = true;ptr.betalt2.disabled = true;
		ptr.rstdlt3.disabled = true ;ptr.alphalt3.disabled = true;ptr.betalt3.disabled = true;
		ptr.rstdlt4.disabled = true ;ptr.alphalt4.disabled = true;ptr.betalt4.disabled = true;
		ptr.rstdlt5.disabled = true ;ptr.alphalt5.disabled = true;ptr.betalt5.disabled = true;
		ptr.rstdlt6.disabled = true ;ptr.alphalt6.disabled = true;ptr.betalt6.disabled = true;
		ptr.rstdlt7.disabled = true ;ptr.alphalt7.disabled = true;ptr.betalt7.disabled = true;
		ptr.rstdlt8.disabled = true ;ptr.alphalt8.disabled = true;ptr.betalt8.disabled = true;
		ptr.rstdlt9.disabled = true ;ptr.alphalt9.disabled = true;ptr.betalt9.disabled = true;
	}
	ptr.cklt1.checked = ("@@cklt1@@" != "on")?false:true;              
	ptr.cklt2.checked = ("@@cklt2@@" != "on")?false:true;					
	ptr.cklt3.checked = ("@@cklt3@@" != "on")?false:true;				
	ptr.cklt4.checked = ("@@cklt4@@" != "on")?false:true;					
	ptr.cklt5.checked = ("@@cklt5@@" != "on")?false:true;				
	ptr.cklt6.checked = ("@@cklt6@@" != "on")?false:true;				
	ptr.cklt7.checked = ("@@cklt7@@" != "on")?false:true;
	ptr.cklt8.checked = ("@@cklt8@@" != "on")?false:true;
	ptr.cklt9.checked = ("@@cklt9@@" != "on")?false:true;
	ptr.confdxcl.checked = ("@@confdxcl@@" != "on")?false:true;
	ptr.confcycs.value = "@@confcycs@@";	
	ptr.confcysj.value = "@@confcysj@@";	
	ptr.confwd.value = "@@confwd@@";
	ptr.zdpselect.value = "@@zdpselect@@";
	ptr.lcselect.value = "@@lcselect@@";
	
   	if (GetCookie('showconftb') == 'true') 
   		confpopUp();	                
   }                                               
   function confsubmit(choice)                    
   {                                               
		//check the validaty first              
	var tbsel=getconftblst();
	var ptr = document.confform;
	if(choice == 'tbcancel')
		return;
	var lstdata="@@conflst@@";		
	document.confform.cmdtype.value='conf_'+choice.substr(2);
	if(choice == 'tbadd')
	{	//confadd
		var newname=window.prompt('请选择一个名字：');
		if(newname ==  null) return;
		if(newname ==  '') return;
		
		if(lstdata.search(newname) >= 0){
			alert('名字('+newname+')已经存在');
			return;
		}
		document.confform.confsz.value=newname;
	}
	if(choice == 'tbdelete')
	{	//confdelete
		if(tbsel.value == 'Default')	return;
		if(tbsel.value == '')		return;
		if(window.confirm('你确定要删除设置-'+tbsel.value+'?') != true )
			return;
		document.confform.confsz.value = tbsel.value;
	}
	if(choice == 'tbupdate')
	{	//confupdate
//		if(tbsel.value == 'Default')		return;
	    if("@@confsz@@" == "Default"){
		ptr.rstdlt1.disabled = false ;ptr.alphalt1.disabled = false;ptr.betalt1.disabled = false;
		ptr.rstdlt2.disabled = false ;ptr.alphalt2.disabled = false;ptr.betalt2.disabled = false;
		ptr.rstdlt3.disabled = false ;ptr.alphalt3.disabled = false;ptr.betalt3.disabled = false;
		ptr.rstdlt4.disabled = false ;ptr.alphalt4.disabled = false;ptr.betalt4.disabled = false;
		ptr.rstdlt5.disabled = false ;ptr.alphalt5.disabled = false;ptr.betalt5.disabled = false;
		ptr.rstdlt6.disabled = false ;ptr.alphalt6.disabled = false;ptr.betalt6.disabled = false;
		ptr.rstdlt7.disabled = false ;ptr.alphalt7.disabled = false;ptr.betalt7.disabled = false;
		ptr.rstdlt8.disabled = false ;ptr.alphalt8.disabled = false;ptr.betalt8.disabled = false;
		ptr.rstdlt9.disabled = false ;ptr.alphalt9.disabled = false;ptr.betalt9.disabled = false;
	}

		if(tbsel.value == '')			return;
		if(lstdata.search(tbsel.value) < 0)	return;
		document.confform.confsz.value=document.confform.tbselect.value;
	}
	if(choice == 'tbclose')
	{
		hidediv('confhtm');
		var lst = document.getElementById('testhtm');
		lst.style.top = 50;
		document.confform.confsz.value = tbsel.value;
		return;
	}
	//confselect
	if(choice == 'tbselect')
	{
		if(tbsel.value == '')			return;
		if(lstdata.search(tbsel.value) < 0)	return;
		document.confform.confsz.value=document.confform.tbselect.value;
	}
	document.confform.submit();
}
function confpopUp(distype)
{
    var tb1 = document.getElementById('confitems');
    if(tb1.style.display == '')
    {
    	tb1.style.display = 'none';
    	SetCookie('showconftb','false');
    }
    else
    {
	tb1.style.display = '';
	SetCookie('showconftb','true');
    }	
}
document.writeln("<form name='confform' method='POST' action='ToHtmlCtrl'>  ");
document.writeln("<table width='50%' border='0' background='texture.jpg' cellspacing='0'><tr><td>");
document.writeln("<div id='conftbdiv' style='position:absolute;width:100; color: #8C6D5A; zindex:0'>");
document.writeln("	 <input type='hidden' name='confsz'>");
document.writeln("       <input type='hidden' name='cmdtype'>");
document.writeln("       <table id='confitems' style='' border = '0'  width='100' cellspacing='0' >  ");
document.writeln("       <tr><td>");
document.writeln("<a href=# ><img name='confImage2' onClick='confpopUp()' value='tbcancel' src='imgorg/pinon.gif' width='20' height='18' ></a>");
document.writeln("      </td>");
document.writeln(" 	<td ><select name='tbselect' onchange=confsubmit('tbselect')></select></td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		<td ><button type='button' onclick=confsubmit('tbadd')>另存为..</td>  ");
document.writeln("		<td ><button type='button' onclick=confsubmit('tbdelete')>删除..</td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		<td ><button type='button' onclick=confsubmit('tbupdate')>修改..</td>  ");
document.writeln("		<td ><button type='button' onclick=confsubmit('tbclose')>关闭..</td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		</tr></table></div>");
document.writeln("<img name='confImagel' onClick='confpopUp()' src='imgorg/pinon.gif' width='20' height='18' >");
document.writeln("</td></tr><tr><td><br>");
document.writeln(" <table width='100%' border='1' background='texture.jpg' bordercolor='#000000'>");
document.writeln("    <tr align='center'> ");
document.writeln("      <td width='25%'>(*Ω)</td>");
document.writeln("      <td width='20%'>&nbsp;</td>");
document.writeln("      <td width='20%'>阻值(*Ω)</td>");
document.writeln("      <td width='20%'>系数α<font size='-1'>(*<span>10<sup><font size='1'>-6</font></sup></span>)</font></td>");
document.writeln("      <td >系数β<font size='-1'>(*<span>10<sup><font size='1'>-6</font></sup></span>)</font></td>");
document.writeln("    </tr>");

document.write("<tr align='center'> <td align='left'>0.001</td>");
document.write("<td ><input type='checkbox' name='cklt9' ></td>");
document.write("<td ><input type='text' name='rstdlt9'></td>");
document.write("<td ><input type='text' name='alphalt9'></td>");
document.write("<td ><input type='text' name='betalt9'></td></tr>");

document.write("<tr align='center'> <td align='left'>0.01</td>");
document.write("<td ><input type='checkbox' name='cklt8' ></td>");
document.write("<td ><input type='text' name='rstdlt8'></td>");
document.write("<td ><input type='text' name='alphalt8'></td>");
document.write("<td ><input type='text' name='betalt8'></td></tr>");


var zxconftblst = "0.1,1,10,100,1K,10K,100K";
var zxconftbarray = zxconftblst.split(',');
var zxconfi;
for(zxconfi = 1;zxconfi <= zxconftbarray.length;zxconfi++)
{
	document.write("<tr align='center'> <td align='left'>"+zxconftbarray[zxconfi-1]+"</td>");
    	document.write("<td ><input type='checkbox' name='cklt"+zxconfi+"' ></td>");
    	document.write("<td ><input type='text' name='rstdlt"+zxconfi+"'></td>");
    	document.write("<td ><input type='text' name='alphalt"+zxconfi+"'></td>");
    	document.write("<td ><input type='text' name='betalt"+zxconfi+"'></td></tr>");
}
document.writeln("    <tr><td colspan='5'>&nbsp</td></tr>");
document.writeln("    <tr > <td colspan='2' rowspan='4'>&nbsp</td> ");
document.writeln("      <td > 单向测量 </td>");
document.writeln("      <td > 最低盘满量程 </td>");
document.writeln("      <td > 纳伏表量程 </td>");
document.writeln("    </tr>");
document.writeln("    <tr > ");
document.writeln("      <td > <input type='checkbox' name='confdxcl'></td>");
document.writeln("      <td > ");
document.writeln("      	  <select name='zdpselect'>");
document.writeln("      	    <option value='0.01'>0.01</option> <option value='0.1'>0.1</option> <option value='1'>1</option> <option value='10'>10</option>");
document.writeln("            <option value='100'>100</option> <option value='1K'>1K</option> <option value='10K'>10K</option> <option>100K</option>");
document.writeln("          </select>");
document.writeln("      </td>");
document.writeln("      <td > ");
document.writeln("          <select name='lcselect'>");
document.writeln("      	    <option value='自动'>自动</option> <option value='10mV'>10mV</option> <option value='100mV'>100mV</option>");
document.writeln("             <option value='1V'>1V</option> <option value='10V'>10V</option> <option value='100V'>100V</option>");
document.writeln("          </select>");
document.writeln("      </td>");
document.writeln("    </tr>");
document.writeln("    <tr > ");
document.writeln("      <td > 采样时间(s) </td>");
document.writeln("      <td > 采样次数 </td>");
document.writeln("      <td > 温度 </td>");
document.writeln("    </tr>");
document.writeln("    <tr> ");
document.writeln("      <td > <input type='text' name='confcysj'> </td>");
document.writeln("      <td > <input type='text' name='confcycs'> </td>");
document.writeln("      <td > <input type='text' name='confwd'> </td>");
document.writeln("    </tr>");

document.writeln("      </table></td></tr></table> ");
document.writeln("      </form>");
