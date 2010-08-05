function loadTestform()
{
	if (document.tbrform.curhtm.value == 'confhtm') {
		testsubmit('tbconfset');
	}
	document.intervalid = setInterval('checkincmd()',100); 
	return;
}
function exitTestform()
{
	var st = document.getElementById('teststatus');
	if((st.value == '暂停@@testprgsz@@测试')){
		window.alert('正在测试中');
		return false;
	}
	hidediv('confhtm');
	clearInterval(document.intervalid);
	return true;
}
function clearhistory(){
		var ctrl;
		ctrl =  document.getElementById('history');
		if(ctrl){
			ctrl.length = 0; 
		}
}
function Updatechart(){
	var ctrl,dbase,dcri;
	ctrl =  document.getElementById('chartbase');
	if(ctrl){dbase = ctrl.value;}
	ctrl =  document.getElementById('chartvary');
	if(ctrl){dcri = ctrl.value;}
	ctrl =  document.getElementById('RTestChart1');
	if(ctrl){
		var up = dbase*1.0+5.0*dcri;
		var dn = dbase*1.0-5.0*dcri;
//		window.alert(up);
//		window.alert(dn);
//		window.alert(dbase);
//		window.alert(4.0*dcri);
		ctrl.SetAxis(up,dn,dbase,4.0*dcri);
	}
}
function Resetchart(){
	var ctrl =  document.getElementById('RTestChart1');
	if(ctrl){
		ctrl.ClearAll();
	}
}
function fillcommit(cmdpos){
	var ctrl = document.getElementById(cmdpos);
	if(ctrl){
		checkincmd();
		window.clipboardData.setData('Text','test-cmd:fillcommit:'+cmdpos+'='+ctrl.value);
	}
}
function testonce(cmdparm){
	checkincmd();
	window.clipboardData.setData('Text','test-cmd:testonce:'+cmdparm);
}
function checkincmd(){
	loadTbrform();
	if(document.tbrform.curview.value != "testhtm"){
		if(document.tbrform.curview.value != "confhtm"){
			return;
		}
	}

	var cmdname = window.clipboardData.getData('Text');

	if(cmdname){
		var cmdcopy;
		cmdcopy = cmdname;
//		window.alert(cmdname);
		if(cmdname == 'rtest:header:done')  cmdname = 'rtest:header:测试结果生成成功';
		if(cmdname == 'rtest:header:fail')  cmdname = 'rtest:header:测试结果生成失败';
		if(cmdname == 'rtest:header:error') cmdname = 'rtest:header:测试出现错误或被取消';

		var cmdlst = cmdname.split(':');
		if(cmdlst.length == 3){
			if(cmdlst[0] == 'rtest'){
				window.clipboardData.clearData('Text');

				var cmdctrl = document.getElementById(cmdlst[1]);
				
				if(cmdctrl){
					cmdctrl.value = cmdlst[2];
					if((cmdlst[1] != 'display')&&(cmdlst[1] != 'header')&&(cmdlst[1] != 'switch')){
						var ctrl;
						ctrl =  document.getElementById('history');
						if(ctrl){
							var all,index;
							all = '';
							for(index=0;index < ctrl.length;index++)
							{
								if(ctrl[index].text){
									all = all+ctrl[index].text+',';
								}	
							}
							all = all+cmdlst[2]+',';
							ctrl.length = ctrl.length+1; 
							var lstarray = all.split(',');//seperate by ','
							ctrl.length = lstarray.length-1;//ignore the last ,
							for (index=0;index< ctrl.length;index++){
								ctrl[index].value=lstarray[index];
								ctrl[index].text=lstarray[index];
							}
						}
					}
					if((cmdlst[1] == 'Z3')&&(cmdlst[2] != 'NA')){
						var ctrl;
						ctrl =  document.getElementById('RTestChart1');
						if(ctrl){ctrl.AddData(cmdlst[2]);}
					}
				}
			}
			if((cmdcopy == 'rtest:header:done')||
			   (cmdcopy == 'rtest:header:fail')||
			   (cmdcopy == 'rtest:header:error')){
			   	var st = document.getElementById('teststatus');
			   	if (st) st.value = '开始@@testprgsz@@测试';
			}
			   
		}
	}
}
function pastecmd(cmdname){
	checkincmd();
	window.clipboardData.setData('Text','test-cmd:'+cmdname);
}
function clearall(){
	var zs,cnti;
	for(cnti=1;cnti<400;cnti++){
		zs = document.getElementById('Z'+cnti);
		if(zs)	zs.value = '';
	}
	
}
function showretest(zindex){
	var st = document.getElementById('teststatus');

		
	if((st.value == '暂停@@testprgsz@@测试')){
		return;
	}
	var zctrl= document.getElementById('Z'+zindex);
	if(zctrl){
		if(zctrl.value == '重测')
			return;		
		hideretest();
		if((zctrl.value == '') || (zctrl.value == ' '))
			return;

		var rval = document.getElementById('retestval');
		var rpos = document.getElementById('retestpos');
		
		rpos.value = 'Z'+zindex;
		rval.value = zctrl.value;
		zctrl.value = '重测';
	}
}
function hideretest(){
	var st = document.getElementById('teststatus');
	if((st.value == '暂停@@testprgsz@@测试')){
		return;
	}
	var rval = document.getElementById('retestval');
	var rpos = document.getElementById('retestpos');
	if((rpos.value != '')){
		var oldctrl = document.getElementById(rpos.value);
		oldctrl.value = rval.value;
		rpos.value = '';
	}
}

function testsubmit(choice)
{

	var st = document.getElementById('teststatus');
	if(choice == 'tbcancel')
		return;
	if(choice == 'tbconfset'){
		if(st.value == '暂停@@testprgsz@@测试')
			return;
		loadconfform();
		mydiv = document.getElementById("confhtm");
		mydiv.style.top = 80;
		hidediv('testhtm');
		return;
	}
	if(choice == 'tbbegin'){
		if(st.value == '开始@@testprgsz@@测试'){
			var st2 = document.getElementById('retestlst');
			st2.value = '';
			clearall();
			st.value = '暂停@@testprgsz@@测试';
			pastecmd('begin');
		}else if(st.value == '暂停@@testprgsz@@测试'){
			pastecmd('pause');
			if(window.confirm('按<确定>继续测试,按<取消>退出测试') == true){
				pastecmd('continue');
			}else{
				pastecmd('abort');
				st.value = '开始@@testprgsz@@测试';
			}
		}
	}
	if(choice  == 'retest'){
		if(st.value == '开始@@testprgsz@@测试'){
			var rval = document.getElementById('retestval');
			var rpos = document.getElementById('retestpos');
			
			if((rpos.value == ''))
				return;
				
			var rcmd= 'retest:'+rpos.value;
			var trval = rval.value;
			var trpos = rpos.value;
			if(window.confirm('确定要重测吗？') != true)
				return;
			var st2 = document.getElementById('retestlst');
			st2.value += (trpos+"="+trval+";");
				
			var st3 = document.getElementById(trpos);
			st3.value = '重测中...';
				
			st.value = '暂停@@testprgsz@@测试';
			pastecmd(rcmd);
			
		}
	}
}
function addzedit(zindex)
{
		document.write("<input type='text' value='' ");
		document.write(" onMouseOver=showretest('"+zindex+"')");
		document.write(" onMouseOut=hideretest()");
		document.write(" onClick=testsubmit('retest')");
		document.write(" onFocus='this.blur()' size='8' class='nbinput' ");
		document.write("id='Z"+zindex+"'>");
}
document.writeln("<table width='100%'><tr >");
document.writeln("<td><input type='text' value='' id='header' onFocus='this.blur()' class='nbheader'>");
document.writeln("<br>");
document.writeln("<button type='button' id='teststatus' onClick=testsubmit('tbbegin')>开始@@testprgsz@@测试</button>");
document.writeln("<font color=green>|</font>");
//document.writeln("<img name='testImagel' onClick=testsubmit('tbconfset') src='imgorg/pinon.gif' width='20' height='18' >");
document.writeln("<button type='button' onClick=testsubmit('tbconfset')>参数设置</button>");
document.writeln("<font color=green>|历史数据</font>");
document.writeln("<select id='history'></select><button type='button' onClick=clearhistory()>清除</button>");
document.writeln("</td></tr><tr><td>");
