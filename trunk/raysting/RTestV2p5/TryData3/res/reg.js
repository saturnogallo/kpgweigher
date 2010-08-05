function getregtblst()
{
	return document.regform.tbselect;
}
function addreglist()
{
	var lstctl = getregtblst();
	var lstdata="@@reglst@@";
	var lstarray = lstdata.split(',');//seperate by ','
	lstctl.length = lstarray.length-1;//ignore the last ,
	var index=0;
	for (index=0;index< lstctl.length;index++){
		lstctl[index].value=lstarray[index];
		lstctl[index].text=lstarray[index];
	}
	lstctl.value = "@@regsz@@";
}
function rqconvert()
{
	var sdate=document.regform.jdrq.value;
	var sarr=sdate.split('-');
	if(sarr.length != 3){
		window.alert("检定日期格式无效");
	}else{
		document.regform.jdrqn.value = sarr[0];
		document.regform.jdrqy.value = sarr[1];
		document.regform.jdrqr.value = sarr[2];
	}
	sdate=document.regform.yxqz.value;
	sarr=sdate.split('-');
	if(sarr.length != 3){
		window.alert("有效期至格式无效");
	}else{
		document.regform.yxqzn.value = sarr[0];
		document.regform.yxqzy.value = sarr[1];
		document.regform.yxqzr.value = sarr[2];
	}		
}
function exitRegform()
{
	var fulltype = document.regform.xhgg.value;
	document.regform.regfulltype.value = fulltype;
	fulltype.toLowerCase();
	//conversion from yxrq 2 yxrq(n,y,r)
	rqconvert();
	return true;
}
function loadRegform()
{
	var formptr = document.regform;
	//showtoolbar('reghtm');
	addreglist();
	//formptr.yysz.value = "@@yysz@@";
	formptr.wtz.value = "@@wtz@@";
	formptr.wtzdz.value = "@@wtzdz@@";
	formptr.qjmc.value = "@@qjmc@@";
	formptr.zzc.value = "@@zzc@@";
	formptr.xhgg.value = "@@xhgg@@";
	formptr.qjbh.value = "@@qjbh@@";
	formptr.qjzqd.value = "@@qjzqd@@";
	formptr.jdjl.value = "@@jdjl@@";
	formptr.jdrq.value = "@@jdrq@@";
	var cdate = new Date();

	var scdate = cdate.getYear()+'-'+cdate.getMonth()+'-'+cdate.getDate();

	if((formptr.jdrq.value == '')||(formptr.jdrq.value == ' ')){
		formptr.jdrq.value = scdate;
	}
	formptr.yxqz.value = "@@yxqz@@";
	if((formptr.yxqz.value == '')||(formptr.yxqz.value == ' ')){
		formptr.yxqz.value = scdate;
	}
	formptr.jsgf.value = "@@jsgf@@";
	formptr.dd.value = "@@dd@@";
	formptr.wd.value = "@@wd@@";
	formptr.sd.value = "@@sd@@";
	formptr.zsbh.value = "@@zsbh@@";
	formptr.xljc.value = "@@xljc@@";
	formptr.jydz.value = "@@jydz@@";
	formptr.gpnysy.value = "@@gpnysy@@";
	formptr.mc1.value = "@@mc1@@";
	formptr.mc2.value = "@@mc2@@";
	formptr.mc3.value = "@@mc3@@";
	formptr.mc4.value = "@@mc4@@";
	formptr.mc5.value = "@@mc5@@";
	formptr.mc6.value = "@@mc6@@";
	formptr.mc7.value = "@@mc7@@";
	formptr.bh1.value = "@@bh1@@";
	formptr.bh2.value = "@@bh2@@";
	formptr.bh3.value = "@@bh3@@";
	formptr.bh4.value = "@@bh4@@";
	formptr.bh5.value = "@@bh5@@";
	formptr.bh6.value = "@@bh6@@";
	formptr.bh7.value = "@@bh7@@";
	formptr.zsbh1.value = "@@zsbh1@@";
	formptr.zsbh2.value = "@@zsbh2@@";
	formptr.zsbh3.value = "@@zsbh3@@";
	formptr.zsbh4.value = "@@zsbh4@@";
	formptr.zsbh5.value = "@@zsbh5@@";
	formptr.zsbh6.value = "@@zsbh6@@";
	formptr.zsbh7.value = "@@zsbh7@@";
	formptr.clfw1.value = "@@clfw1@@";
	formptr.clfw2.value = "@@clfw2@@";
	formptr.clfw3.value = "@@clfw3@@";
	formptr.clfw4.value = "@@clfw4@@";
	formptr.clfw5.value = "@@clfw5@@";
	formptr.clfw6.value = "@@clfw6@@";
	formptr.clfw7.value = "@@clfw7@@";
	
	if (GetCookie('showregtb') == 'true')
		regpopUp();
}
function regsubmit(choice)
{
	var tbsel = getregtblst();
	//check the validaty first
	if(choice == 'tbcancel')
		return;
	
	document.regform.regsz.value = tbsel.value;	
	if(choice == 'tbadd')
	{	//regadd
		var newname=window.prompt('请选择一个名字：');
		if(newname ==  null) return;
		if(newname ==  '') return;
		var lstdata="@@reglst@@";
		if(lstdata.search(newname) >= 0){
			alert('名字('+newname+')已经存在');
			return;
		}
		document.regform.cmdtype.value='reg_add';
		document.regform.regsz.value=newname;
		document.regform.submit();
		return;
	}
	if(choice == 'tbdelete')
	{	//regdelete
		if(tbsel.value == "")	return;
		if(window.confirm('你确定要删除设置-'+tbsel.value+'?') == true )
		{
			document.regform.cmdtype.value='reg_delete';
			document.regform.submit();
			return;
		}
	}
	if(choice == 'tbupdate')
	{	//regupdate
		if(tbsel.value == "")	return;
		document.regform.cmdtype.value='reg_update';
		document.regform.submit();
		return;
	}
	//regselect
	var lstdata="@@reglst@@";
	if(lstdata.search(newname) >= 0)
	{
		if(tbsel.value == "")	return;
		document.regform.cmdtype.value='reg_select';
		document.regform.submit();
		return;
	}
}
function regpopUp()
{
    newX = window.event.x + document.body.scrollLeft;
    newY = window.event.y + document.body.scrollTop;
    var tb1 = document.getElementById('regitems');
    if(tb1.style.display == '')
    {
    	tb1.style.display = 'none';
    	SetCookie('showregtb','false');
    }
    else
    {
	tb1.style.display = '';
	SetCookie('showregtb','true');
    }	
}
document.writeln("<form name='regform' method='POST' action='ToHtmlCtrl'>  ");
document.writeln("    	<input type='hidden' name='cmdtype' >");
document.writeln("    	<input type='hidden' name='regfulltype' >");
document.writeln("    	<input type='hidden' name='regsz' >");
document.writeln("<table border = '0' cellspacing='0'>  ");
document.writeln("<tr >  ");
document.writeln("	  <td width='48' rowspan='10'>   ");
document.writeln("      </td>  ");
document.writeln("	  <td width='48' rowspan='10'>   ");
document.writeln("</td></tr>  ");
document.writeln("      <td colspan='6' >  ");

document.writeln("<a href=# ><img name='regImagel' onClick='regpopUp()' value='tbcancel' src='imgorg/pinon.gif' width='20' height='18' ></a>");
document.writeln("<div id='regtbdiv' style='position:absolute; width:100; color: #8C6D5A; zindex:0'>");
document.writeln("       <table id='regitems' style='' border = '0' background='texture.jpg' width='100' cellspacing='0' >  ");
document.writeln("        <tr valign='bottom'>  <td width='20%'></td> ");
document.writeln("        	<td ><select name='tbselect' onchange=regsubmit('tbselect')></select></td>  ");
document.writeln("		<td ><button type='button' name='tbupdate' onclick=regsubmit('tbupdate')>更新设置</td>  ");
document.writeln("		<td ><button type='button' name='tbadd' onclick=regsubmit('tbadd')>新建为..</td>  ");
document.writeln("		<td ><button type='button' name='tbdelete' onclick=regsubmit('tbdelete')>删除设置</td>  ");
document.writeln("		</tr></table></div>");

document.writeln("        <table border='1' cellspacing='0' width='644' bordercolor='#000000'  >  ");
document.writeln("  </tr>  ");
document.writeln("      <td>证书编号：</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='zsbh'  ></td>  ");
document.writeln("  	  <td>委托者</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='wtz' ></td>  ");
document.writeln("          </tr>  ");
document.writeln("          <tr>  ");
document.writeln("      <td>委托者地址</td>  ");
document.writeln("      <td  >  ");
document.writeln("        <input type='text' class='reginput' name='wtzdz' >  ");
document.writeln("      </td>  ");
document.writeln("  	  <td>器具名称</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='qjmc' ></td>  ");
document.writeln("  </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td>制造厂</td>  ");
document.writeln("      <td  ><input type='text' class='reginput' name='zzc' ></td>  ");
document.writeln("  	  <td >型号/规格</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='xhgg' ></td>  ");
document.writeln("  </tr>  ");
document.writeln("   <tr>  ");
document.writeln("      <td>器具编号</td>  ");
document.writeln("      <td  ><input type='text' class='reginput' name='qjbh' ></td>  ");
document.writeln("  	  <td >器具准确度</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='qjzqd'></td>  ");
document.writeln("  </tr>  ");
document.writeln("     <tr>  ");
document.writeln("      <td > 外观及线路检查</td>  ");
document.writeln("      <td  ><input type='text'  class='reginput' name='xljc'  ></td>  ");
document.writeln("      <td>检定结论</td>  ");
document.writeln("      <td  ><input type='text' class='reginput' name='jdjl'></td>  ");
document.writeln("  </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td>检定日期</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='jdrq' > </td>  ");
document.writeln("		<input type='hidden' name='jdrqn'>	");
document.writeln("		<input type='hidden' name='jdrqy'>	");
document.writeln("		<input type='hidden' name='jdrqr'>	");
document.writeln("      <td>有效期至</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='yxqz' > </td>  ");
document.writeln("		<input type='hidden' name='yxqzn'>	");
document.writeln("		<input type='hidden' name='yxqzy'>	");
document.writeln("		<input type='hidden' name='yxqzr'>	");
document.writeln("  </tr>  ");
document.writeln("    ");
document.writeln("   <tr>  ");
document.writeln("      <td colspan='1'>依据的技术规范</td>  ");
document.writeln("      <td colspan='1'><input type='text' class='reginput' name='jsgf' ></td>  ");
document.writeln("      <td colspan='1'>地点</td>  ");
document.writeln("	  <td colspan='1'><input type='text' class='reginput' name='dd' ></td>  ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td>温度:(℃)</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='wd'  ></td>  ");
document.writeln("      <td>湿度:(%RH)</td>  ");
document.writeln("	  <td  ><input type='text' class='reginput' name='sd'  ></td>	    ");
document.writeln("  </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td > 绝缘电阻</td>  ");
document.writeln("      <td  ><input type='text' class='reginput' name='jydz' ></td>  ");
document.writeln("      <td > 工频耐压试验</td>  ");
document.writeln("      <td  ><input type='text' class='reginput' name='gpnysy' ></td>  ");
document.writeln("  </tr>  ");
document.writeln("    <tr align='center'>   ");
document.writeln("      <td  colspan = '6'>本次检定所使用的主要计量标准器具： </td>  ");
document.writeln("	</tr>  ");
document.writeln("  <tr>  ");
document.writeln("  ");
document.writeln("      <td   align='center'>名称/型号</td>  ");
document.writeln("      <td > <p align='center'>编号</p></td>  ");
document.writeln("      <td  > <p align='center'>证书编号/有效期限  </p>	</td>             ");
document.writeln("      <td > <p align='center'>测量范围/准确度  </td>            ");
document.writeln("  </tr>  ");
document.writeln("    ");
document.writeln("  ");
document.writeln("	<tr>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='mc1' ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh1'  ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh1' ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw1'></td>	    ");
document.writeln("  </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc2'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh2'  ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh2' ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw2'  ></td>	    ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc3'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh3' ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh3'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw3'  ></td>	    ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc4'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh4' ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh4'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw4'  ></td>	    ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc5'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh5'  ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh5'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw5' ></td>	    ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc6'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh6'  ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh6' ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw6' ></td>	    ");
document.writeln(" </tr>  ");
document.writeln("  <tr>  ");
document.writeln("      <td  > <input type='text' class='reginput' name='mc7'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='bh7'  ></td>  ");
document.writeln("	  <td  > <input type='text' class='reginput' name='zsbh7'  ></td>  ");
document.writeln("      <td > <input type='text' class='reginput' name='clfw7' ><input type='hidden' name='null'></td>	    ");
document.writeln(" </tr>  ");
document.writeln("</table>  ");
document.writeln("</td>  ");
document.writeln("</table>  ");
document.writeln("</form>  ");

//false Exit htm just for exit
function loadExitform()
{
	document.regform.cmdtype.value='exit';
	document.regform.submit();
}
