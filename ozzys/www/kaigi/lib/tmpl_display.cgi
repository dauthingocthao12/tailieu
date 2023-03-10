

<!--■ 管理用テンプレートファイル -->
<!--■ 管理用なので、改造しない方が無難です。 -->



<!--▼一部のヘッダブロック-->
<!--BLOCK="header"-->
<html>
<head>
	<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>/?title?/::管理用/?sub_title?/</title>
	<style type="text/css">
	<!--
	body {
		background-color:/?theme_color0?/;
		color:#000000;
		font-size:/?admin_font?/;
		margin:0px;
	}
	td,th { font-size:/?admin_font?/; }
	.box0 { background-color:#FFFFFF; }
	.box1 {
		background-color:/?theme_color1?/;
		padding:1em;
		color:/?theme_color2?/;
	}
	.box2 { background-color:#CCCCCC;padding:0.5em; }
	.box3 { background-color:#EEEEEE;padding:0.5em; }
	.c1   { color:/?theme_color3?/; }
	.stc  { color:#FF0000; }
	small { color:#666666; }
	a  {text-decoration:none;color:#0000FF;}
	a:visited {text-decoration:none;color:#0000FF;}
	-->
	</style>
<!--/BLOCK="header"-->




<!--▼ヘッダ～フォームブロック-->
<!--BLOCK="display"-->

	<script language="javascript">
	<!--
	
	// 色関係の初期化
	TGC = '0';
	
	// テキストエリア選択
	function select_area(cval) {
		if     (TGC == 0) { document.palette.bg_color.value = cval; }
		else if(TGC == 1) { document.palette.font_color.value = cval; }
		else if(TGC == 2) { document.palette.strong_color.value = cval; }
		else if(TGC == 3) { document.palette.faint_color.value = cval; }
		else if(TGC == 4) { document.palette.strong_color2.value = cval; }
		else if(TGC == 5) { document.palette.new_color.value = cval; }
		else if(TGC == 8) { document.palette.link_color.value = cval; }
		else if(TGC == 9) { document.palette.vlink_color.value = cval; }
		showsample();
	}
	
	//見本の表示
	function showsample() {
		self.ifr.document.open();
		self.ifr.document.writeln(
			'<html><head>',
			'<title>SAMPLE</title>'
		);
		self.ifr.document.writeln('<style type="text/css"><!--');
		self.ifr.document.writeln(
			'body { background-color:',
			document.palette.bg_color.value,
			';font-size:12px;color:',
			document.palette.font_color.value,
			';}'
		);
		self.ifr.document.writeln('input{font-size:12px;}');
		self.ifr.document.writeln('.stc{color:',document.palette.strong_color.value,';}');
		self.ifr.document.writeln('.stc2{color:',document.palette.strong_color2.value,';}');
		self.ifr.document.writeln('.new{color:',document.palette.new_color.value,';}');
		self.ifr.document.writeln('.ftc{color:',document.palette.faint_color.value,';}');
		self.ifr.document.writeln('.ln{color:',document.palette.link_color.value,';}');
		self.ifr.document.writeln('.vln{color:',document.palette.vlink_color.value,';}');
		self.ifr.document.writeln('-','-></style>');
		if(
			document.palette.title_img.value == ''
			||
			document.palette.title_img.value == 'http://'
		) {
			TITLE = 'タイトル';
		}
		else {
			TITLE = '<img src="'+document.palette.title_img.value+'" alt="タイトル">';
		}
		if(
			document.palette.folder_img.value == ''
			||
			document.palette.folder_img.value == 'http://'
		) {
			FOLDER = '<font class="ln">▼</font>';
		}
		else {
			FOLDER = '<img src="'+document.palette.folder_img.value+'">';
		}
		if(
			document.palette.doc_img.value == ''
			||
			document.palette.doc_img.value == 'http://'
		) {
			DOCIMG = '';
		}
		else {
			DOCIMG = '<img src="'+document.palette.doc_img.value+'"">';
		}
		
		//見本のHTML
		self.ifr.document.write(
			'<body background=',document.palette.bg_img.value,'>',
			'<b><big>',TITLE,'</big></b>',
			'<hr>',
			'<font>文字色</b></font>',
			'<input type="text" size=6 value="TEXT">',
			'<input type="button" value="ボタン">',
			'<hr>',
			'<font class="ln">リンク色</font> ｜ <font class="vln">訪問済色</font>',
			'<hr>',
			'<font class="stc"><b>■ 強調色１</b></font><p></p>',
			FOLDER+' ',
			DOCIMG+' ',
			'<font class="stc2"><b>強調色２</b></font>',
			'　<font class="new">新着色 </font>',
			'<p></p>',
			'サンプルテキストサンプルテキストサンプルテキスト',
			'サンプルテキストサンプルテキストサンプルテキスト',
			'<div align=right class="ftc">抑制色</div>',
			'<hr>',
			'</body></html>'
		);
		self.ifr.document.close();
	}
	
	//色など全て戻す
	function default_color(RESET) {
		
		//背景
		document.palette.bg_color.value = '/?bg_color?/';
		
		//文字
		document.palette.font_color.value = '/?font_color?/';
		
		//強調1
		document.palette.strong_color.value = '/?strong_color?/';
		
		//抑制
		document.palette.faint_color.value = '/?faint_color?/';
		
		//強調2
		document.palette.strong_color2.value = '/?strong_color2?/';
		
		//新着
		document.palette.new_color.value = '/?new_color?/';
		
		//リンク
		document.palette.link_color.value = '/?link_color?/';
		
		//訪問済
		document.palette.vlink_color.value = '/?vlink_color?/';
		
		//タイトル画像
		//document.palette.title_img.value = '/?title_img?/';
		
		//背景画像
		//document.palette.bg_img.value = '/?bg_img?/';
		
		//フォルダ画像
		//document.palette.folder_img.value = '/?folder_img?/';
		
		//文書画像
		//document.palette.doc_img.value = '/?doc_img?/';
		
		showsample();
		if(RESET == 1) {TGC = 0;}
	}
	
	//画像解除
	function undef_img(TGIMG) {
		//
		if(TGIMG == 'title_img') { document.palette.title_img.value = 'http://'; }
		if(TGIMG == 'bg_img') { document.palette.bg_img.value = 'http://'; }
		if(TGIMG == 'folder_img') { document.palette.folder_img.value = 'http://'; }
		if(TGIMG == 'doc_img') { document.palette.doc_img.value = 'http://'; }
		showsample();
	}
	
	//画像を戻す
	function reset_img(TGIMG) {
		//
		if(TGIMG == 'title_img') { document.palette.title_img.value = '/?title_img?/'; }
		if(TGIMG == 'bg_img') { document.palette.bg_img.value = '/?bg_img?/'; }
		if(TGIMG == 'folder_img') { document.palette.folder_img.value = '/?folder_img?/'; }
		if(TGIMG == 'doc_img') { document.palette.doc_img.value = '/?doc_img?/'; }
		showsample();
	}
	// -->
	</script>
</head>

<body onLoad="showsample()">
<center>
<!-- テーブル始点 -->
<table width="100%" cellpadding=14 cellspacing=0 border=0>
<tr>
	<th class="box1"><big>Powerd by /?this_name?/</big></th>
</tr>
</table>
<!-- テーブル終点 -->


<!-- テーブル始点 -->
<table width="100%" cellpadding=6 cellspacing=0 border=0>
<tr>
	<td class="box2">[<a href="?">▲記事リストへ</a>] &gt; <a href="?m=/?admin_key?/&">管理用</a> &gt; <u>表示設定</u></td>
</tr>
</table>
<!-- テーブル終点 -->


<br>
<p></p>
<form method="POST" name="palette">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="display2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">


<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td colspan="2" class="box2">▼基本設定</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">タイトル</b><br>
	<input type="text" size=50 name="title" value="/?title?/">
	<br><small>（掲示板のタイトルです。例:私の掲示板）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">ホームページURL</b><br>
	<input type="text" size=50 name="home" value="/?home?/">
	<br><small>（管理者"ご自分"のホームページURLです。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box2">▼新着マーク設定</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">新着マーク有効時間</b><br>
	<select name="new_time">
	<option value="0"/?new_time.0?/>×新着マーク無効</option>
	<option value="1"/?new_time.1?/>1時間</option>
	<option value="3"/?new_time.3?/>3時間</option>
	<option value="6"/?new_time.6?/>6時間</option>
	<option value="12"/?new_time.12?/>12時間</option>
	<option value="24"/?new_time.24?/>24時間（１日）</option>
	<option value="48"/?new_time.48?/>48時間（２日）</option>
	<option value="72"/?new_time.72?/>72時間（３日）</option>
	<option value="96"/?new_time.96?/>96時間（４日）</option>
	<option value="168"/?new_time.168?/>168時間（１週間）</option>
	<option value="336"/?new_time.336?/>336時間（２週間）</option>
	<option value="672"/?new_time.672?/>672時間（４週間）</option>
	</select>
	<br><small>（投稿されてからの有効時間内に新着マークが付きます。いつでも変更できます。）</small>
	</td>
</tr><tr>
	<td colspan="2" class="box3">
	<b class="c1">新着マーク</b><br>
	<select name="new_mark">
	<option value="新着!"/?new_mark.新着!?/>新着!</option>
	<option value="ニュー!"/?new_mark.ニュー!?/>ニュー!</option>
	<option value="new!"/?new_mark.new!?/>new!</option>
	<option value="NEW!"/?new_mark.NEW!?/>NEW!</option>
	<option value="にゅぅ!"/?new_mark.にゅぅ!?/>にゅぅ!</option>
	<option value="今が旬!"/?new_mark.今が旬!?/>今が旬!</option>
	<option value="食べ頃!"/?new_mark.食べ頃!?/>食べ頃!</option>
	<option value="♪"/?new_mark.♪?/>♪</option>
	<option value="ν"/?new_mark.ν?/>ν</option>
	</select>
	<br><small>（いつでも変更できます。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box2">▼一般表示設定</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">フォントサイズ</b><br>
	<select name="font_size">
	<option value="8"/?font_size.8?/>8</option>
	<option value="9"/?font_size.9?/>9</option>
	<option value="10"/?font_size.10?/>10</option>
	<option value="11"/?font_size.11?/>11</option>
	<option value="12"/?font_size.12?/>12</option>
	<option value="13"/?font_size.13?/>13</option>
	<option value="14"/?font_size.14?/>14</option>
	<option value="16"/?font_size.16?/>16</option>
	<option value="18"/?font_size.18?/>18</option>
	<option value="24"/?font_size.24?/>24</option>
	</select>
	-
	<select name="font_unit">
	<option value="pt"/?font_unit.pt?/>単位:pt（ブラウザ依存サイズ）</option>
	<option value="px"/?font_unit.px?/>単位:px（固定サイズ）</option>
	</select>
	<br><small>（フォントサイズです。スタイルシート形式で指定できます。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">リンクの下線表示</b><br>
	<select name="link_line">
	<option value="none"/?link_line.none?/>下線無し</option>
	<option value="underline"/?link_line.underline?/>下線有り</option>
	</select>
	<br><small>（リンクの表示形式です。表示例:<a>下線無しリンク</a>／<a><u>下線有りリンク</u></a>）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">日付表示形式</b><br>
	<select name="date_format">
	<option value="A"/?date_format.A?/>Ａタイプ（例：2100年01月12日12時15分(日) ）</option>
	<option value="B"/?date_format.B?/>Ｂタイプ（例：01月12日12時15分(日) ）</option>
	<option value="C"/?date_format.C?/>Ｃタイプ（例：2100/01/12/12:15 (日) ）</option>
	<option value="D"/?date_format.D?/>Ｄタイプ（例：01/12/12:15 (日) ）</option>
	</select>
	<br><small>（日付けの表示形式です。いつでも変更できます。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">ホスト情報の表示設定</b><br>
	<select name="exhibit_host">
	<option value="0"/?exhibit_host.0?/>表示する</option>
	<option value="1"/?exhibit_host.1?/>表示しない（HTMLコメント）</option>
	<option value="2"/?exhibit_host.2?/>表示しない（非公開）</option>
	</select>
	<br><small>（ホスト情報の表示、非表示を選べます。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3" width="100%">
	<b class="c1">オートリンク</b><br>
	<select name="auto_link">
	<option value="0"/?auto_link.0?/>無効</option>
	<option value="1"/?auto_link.1?/>有効（※お勧め）</option>
	</select>
	<br><small>（メッセージに含まれるURLを自動でリンクに変換します。いつでも変更できます。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3" width="100%">
	<b class="c1">外部リンクのターゲットウィンドウ</b><br>
	<select name="target_window">
	<option value="_self"/?target_window._self?/>現在のウィンドウ（_SELF/ノーマル）</option>
	<option value="_top"/?target_window._top?/>現在のウィンドウ（_TOP/フレーム破棄）</option>
	<option value="_blank"/?target_window._blank?/>新しいウィンドウ（_BLANK）</option>
	</select>
	<br><small>（URLやオートリンクなど開くウィンドウの設定です。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">保存記事件数情報</b><br>
	<select name="recs_count">
	<option value="1"/?recs_count.1?/>表示する（※標準）</option>
	<option value="0"/?recs_count.0?/>表示しない</option>
	</select>
	<br><small>（保存中の記事の全件数を表示するかどうかの設定です。保存記事が多い場合、"表示しない"を設定すると、やや処理速度が向上します。）</small>
	</td>
</tr>
<tr>
	<td colspan="2" class="box2">▼画像とカラーパレット設定</td>
</tr>
<tr>
	<td colspan="2" class="box3"><small>▼画像（ブラウザでアクセス可能なアドレスで指定できます。確認ボタンで、確認表示フレームに表示されます。）</small></td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">タイトル画像アドレス</b><br>
	<input type="text" size=50 name="title_img" value="/?title_img?/"><input type="button" value="確認" onClick="showsample();"><input type="button" value="解除" onClick="undef_img('title_img');"><input type="button" value="戻す" onClick="reset_img('title_img');">
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">背景画像アドレス</b><br>
	<input type="text" size=50 name="bg_img" value="/?bg_img?/"><input type="button" value="確認" onClick="showsample();"><input type="button" value="解除" onClick="undef_img('bg_img');"><input type="button" value="戻す" onClick="reset_img('bg_img');">
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">フォルダアイコンアドレス</b><br>
	<input type="text" size=50 name="folder_img" value="/?folder_img?/"><input type="button" value="確認" onClick="showsample();"><input type="button" value="解除" onClick="undef_img('folder_img');"><input type="button" value="戻す" onClick="reset_img('folder_img');">
	</td>
</tr>
<tr>
	<td colspan="2" class="box3">
	<b class="c1">文書アイコンアドレス</b><br>
	<input type="text" size=50 name="doc_img" value="/?doc_img?/"><input type="button" value="確認" onClick="showsample();"><input type="button" value="解除" onClick="undef_img('doc_img');"><input type="button" value="戻す" onClick="reset_img('doc_img');">
	</td>
</tr>

<!-- パレット始点 -->
<tr>
	<td colspan="2" class="box3"><small>▼パレット</small></td>
</tr>
<tr>
<td  colspan="2" class="box3">
<table border=0 cellspacing="1" cellpadding="0">
<tr>
<td><input type="radio" name="hoge" value="bg_color" onClick="TGC=0" checked>背景色</td>
<td><input type="text" size=12 name="bg_color" value="/?bg_color?/" style="font-size:12px"></td>

<td>&nbsp;&nbsp;<input type="radio" name="hoge" value="font_color" onClick="TGC=1">文字色</td>
<td><input type="text" size=12 name="font_color" value="/?font_color?/" style="font-size:12px"></td>
<!-- ボタン -->
<td rowspan="4" align="right" valign="bottom">
&nbsp;&nbsp;<input type="button" value="　手動確認　" onClick="showsample();"><br>
&nbsp;&nbsp;<input type="button" value="　色を戻す　" onClick="default_color();">
</td>
</tr>

<tr>
<td><input type="radio" name="hoge" value="strong_color" onClick="TGC=2">強調色１</td>
<td><input type="text" size=12 name="strong_color" value="/?strong_color?/" style="font-size:12px"></td>

<td>&nbsp;&nbsp;<input type="radio" name="hoge" value="strong_color2" onClick="TGC=4">強調色２</td>
<td><input type="text" size=12 name="strong_color2" value="/?strong_color2?/" style="font-size:12px"></td>
</tr>

<tr>
<td><input type="radio" name="hoge" value="link_color" onClick="TGC=8">リンク色</td>
<td><input type="text" size=12 name="link_color" value="/?link_color?/" style="font-size:12px"></td>

<td>&nbsp;&nbsp;<input type="radio" name="hoge" value="vlink_color" onClick="TGC=9">訪問済色</td>
<td><input type="text" size=12 name="vlink_color" value="/?vlink_color?/" style="font-size:12px"></td>
</tr>

<tr>
<td><input type="radio" name="hoge" value="new_color" onClick="TGC=5">新着色</td>
<td><input type="text" size=12 name="new_color" value="/?new_color?/" style="font-size:12px"></td>

<td>&nbsp;&nbsp;<input type="radio" name="hoge" value="faint_color" onClick="TGC=3">抑制色</td>
<td><input type="text" size=12 name="faint_color" value="/?faint_color?/" style="font-size:12px"></td>
</tr>

</table>

</td>
</tr>
<!-- パレット終点 -->

<!-- カラーチャート始点 -->
<tr>
<td style="padding:0.5em;" width="271" class="box3"><small>▼カラーチャート</small></td>
<!-- 表示部分見出し -->
<td style="padding:0.5em;" width="90%" class="box3"><small>▼確認表示</small></td>
</tr>
<tr>
<td valign="top" class="box3" width="271">
<map name="color_map">
<area shape="rect" coords="1,1,15,15" href="javascript:select_area('#000000')">
<area shape="rect" coords="16,1,30,15" href="javascript:select_area('#000033')">
<area shape="rect" coords="31,1,45,15" href="javascript:select_area('#000066')">
<area shape="rect" coords="46,1,60,15" href="javascript:select_area('#000099')">
<area shape="rect" coords="61,1,75,15" href="javascript:select_area('#0000CC')">
<area shape="rect" coords="76,1,90,15" href="javascript:select_area('#0000FF')">

<area shape="rect" coords="91,1,105,15" href="javascript:select_area('#003300')">
<area shape="rect" coords="106,1,120,15" href="javascript:select_area('#003333')">
<area shape="rect" coords="121,1,135,15" href="javascript:select_area('#003366')">
<area shape="rect" coords="136,1,150,15" href="javascript:select_area('#003399')">
<area shape="rect" coords="151,1,165,15" href="javascript:select_area('#0033CC')">
<area shape="rect" coords="166,1,180,15" href="javascript:select_area('#0033FF')">

<area shape="rect" coords="181,1,195,15" href="javascript:select_area('#006600')">
<area shape="rect" coords="196,1,210,15" href="javascript:select_area('#006633')">
<area shape="rect" coords="211,1,225,15" href="javascript:select_area('#006666')">
<area shape="rect" coords="226,1,240,15" href="javascript:select_area('#006699')">
<area shape="rect" coords="241,1,255,15" href="javascript:select_area('#0066CC')">
<area shape="rect" coords="256,1,270,15" href="javascript:select_area('#0066FF')">

<!-- 区切 -->

<area shape="rect" coords="1,16,15,30" href="javascript:select_area('#330000')">
<area shape="rect" coords="16,16,30,30" href="javascript:select_area('#330033')">
<area shape="rect" coords="31,16,45,30" href="javascript:select_area('#330066')">
<area shape="rect" coords="46,16,60,30" href="javascript:select_area('#330099')">
<area shape="rect" coords="61,16,75,30" href="javascript:select_area('#3300CC')">
<area shape="rect" coords="76,16,90,30" href="javascript:select_area('#3300FF')">

<area shape="rect" coords="91,16,105,30" href="javascript:select_area('#333300')">
<area shape="rect" coords="106,16,120,30" href="javascript:select_area('#333333')">
<area shape="rect" coords="121,16,135,30" href="javascript:select_area('#333366')">
<area shape="rect" coords="136,16,150,30" href="javascript:select_area('#333399')">
<area shape="rect" coords="151,16,165,30" href="javascript:select_area('#3333CC')">
<area shape="rect" coords="166,16,180,30" href="javascript:select_area('#3333FF')">

<area shape="rect" coords="181,16,195,30" href="javascript:select_area('#336600')">
<area shape="rect" coords="196,16,210,30" href="javascript:select_area('#336633')">
<area shape="rect" coords="211,16,225,30" href="javascript:select_area('#336666')">
<area shape="rect" coords="226,16,240,30" href="javascript:select_area('#336699')">
<area shape="rect" coords="241,16,255,30" href="javascript:select_area('#3366CC')">
<area shape="rect" coords="256,16,270,30" href="javascript:select_area('#3366FF')">

<!-- 区切 -->

<area shape="rect" coords="1,31,15,45" href="javascript:select_area('#660000')">
<area shape="rect" coords="16,31,30,45" href="javascript:select_area('#660033')">
<area shape="rect" coords="31,31,45,45" href="javascript:select_area('#660066')">
<area shape="rect" coords="46,31,60,45" href="javascript:select_area('#660099')">
<area shape="rect" coords="61,31,75,45" href="javascript:select_area('#6600CC')">
<area shape="rect" coords="76,31,90,45" href="javascript:select_area('#6600FF')">

<area shape="rect" coords="91,31,105,45" href="javascript:select_area('#663300')">
<area shape="rect" coords="106,31,120,45" href="javascript:select_area('#663333')">
<area shape="rect" coords="121,31,135,45" href="javascript:select_area('#663366')">
<area shape="rect" coords="136,31,150,45" href="javascript:select_area('#663399')">
<area shape="rect" coords="151,31,165,45" href="javascript:select_area('#6633CC')">
<area shape="rect" coords="166,31,180,45" href="javascript:select_area('#6633FF')">

<area shape="rect" coords="181,31,195,45" href="javascript:select_area('#666600')">
<area shape="rect" coords="196,31,210,45" href="javascript:select_area('#666633')">
<area shape="rect" coords="211,31,225,45" href="javascript:select_area('#666666')">
<area shape="rect" coords="226,31,240,45" href="javascript:select_area('#666699')">
<area shape="rect" coords="241,31,255,45" href="javascript:select_area('#6666CC')">
<area shape="rect" coords="256,31,270,45" href="javascript:select_area('#6666FF')">

<!-- 区切 -->

<area shape="rect" coords="1,46,15,60" href="javascript:select_area('#990000')">
<area shape="rect" coords="16,46,30,60" href="javascript:select_area('#990033')">
<area shape="rect" coords="31,46,45,60" href="javascript:select_area('#990066')">
<area shape="rect" coords="46,46,60,60" href="javascript:select_area('#990099')">
<area shape="rect" coords="61,46,75,60" href="javascript:select_area('#9900CC')">
<area shape="rect" coords="76,46,90,60" href="javascript:select_area('#9900FF')">

<area shape="rect" coords="91,46,105,60" href="javascript:select_area('#993300')">
<area shape="rect" coords="106,46,120,60" href="javascript:select_area('#993333')">
<area shape="rect" coords="121,46,135,60" href="javascript:select_area('#993366')">
<area shape="rect" coords="136,46,150,60" href="javascript:select_area('#993399')">
<area shape="rect" coords="151,46,165,60" href="javascript:select_area('#9933CC')">
<area shape="rect" coords="166,46,180,60" href="javascript:select_area('#9933FF')">

<area shape="rect" coords="181,46,195,60" href="javascript:select_area('#996600')">
<area shape="rect" coords="196,46,210,60" href="javascript:select_area('#996633')">
<area shape="rect" coords="211,46,225,60" href="javascript:select_area('#996666')">
<area shape="rect" coords="226,46,240,60" href="javascript:select_area('#996699')">
<area shape="rect" coords="241,46,255,60" href="javascript:select_area('#9966CC')">
<area shape="rect" coords="256,46,270,60" href="javascript:select_area('#9966FF')">

<!-- 区切 -->

<area shape="rect" coords="1,61,15,75" href="javascript:select_area('#CC0000')">
<area shape="rect" coords="16,61,30,75" href="javascript:select_area('#CC0033')">
<area shape="rect" coords="31,61,45,75" href="javascript:select_area('#CC0066')">
<area shape="rect" coords="46,61,60,75" href="javascript:select_area('#CC0099')">
<area shape="rect" coords="61,61,75,75" href="javascript:select_area('#CC00CC')">
<area shape="rect" coords="76,61,90,75" href="javascript:select_area('#CC00FF')">

<area shape="rect" coords="91,61,105,75" href="javascript:select_area('#CC3300')">
<area shape="rect" coords="106,61,120,75" href="javascript:select_area('#CC3333')">
<area shape="rect" coords="121,61,135,75" href="javascript:select_area('#CC3366')">
<area shape="rect" coords="136,61,150,75" href="javascript:select_area('#CC3399')">
<area shape="rect" coords="151,61,165,75" href="javascript:select_area('#CC33CC')">
<area shape="rect" coords="166,61,180,75" href="javascript:select_area('#CC33FF')">

<area shape="rect" coords="181,61,195,75" href="javascript:select_area('#CC6600')">
<area shape="rect" coords="196,61,210,75" href="javascript:select_area('#CC6633')">
<area shape="rect" coords="211,61,225,75" href="javascript:select_area('#CC6666')">
<area shape="rect" coords="226,61,240,75" href="javascript:select_area('#CC6699')">
<area shape="rect" coords="241,61,255,75" href="javascript:select_area('#CC66CC')">
<area shape="rect" coords="256,61,270,75" href="javascript:select_area('#CC66FF')">


<!-- 区切 -->

<area shape="rect" coords="1,76,15,90" href="javascript:select_area('#FF0000')">
<area shape="rect" coords="16,76,30,90" href="javascript:select_area('#FF0033')">
<area shape="rect" coords="31,76,45,90" href="javascript:select_area('#FF0066')">
<area shape="rect" coords="46,76,60,90" href="javascript:select_area('#FF0099')">
<area shape="rect" coords="61,76,75,90" href="javascript:select_area('#FF00CC')">
<area shape="rect" coords="76,76,90,90" href="javascript:select_area('#FF00FF')">

<area shape="rect" coords="91,76,105,90" href="javascript:select_area('#FF3300')">
<area shape="rect" coords="106,76,120,90" href="javascript:select_area('#FF3333')">
<area shape="rect" coords="121,76,135,90" href="javascript:select_area('#FF3366')">
<area shape="rect" coords="136,76,150,90" href="javascript:select_area('#FF3399')">
<area shape="rect" coords="151,76,165,90" href="javascript:select_area('#FF33CC')">
<area shape="rect" coords="166,76,180,90" href="javascript:select_area('#FF33FF')">

<area shape="rect" coords="181,76,195,90" href="javascript:select_area('#FF6600')">
<area shape="rect" coords="196,76,210,90" href="javascript:select_area('#FF6633')">
<area shape="rect" coords="211,76,225,90" href="javascript:select_area('#FF6666')">
<area shape="rect" coords="226,76,240,90" href="javascript:select_area('#FF6699')">
<area shape="rect" coords="241,76,255,90" href="javascript:select_area('#FF66CC')">
<area shape="rect" coords="256,76,270,90" href="javascript:select_area('#FF66FF')">


<!-- ２段目 -->
<!-- 区切 -->

<area shape="rect" coords="1,91,15,105" href="javascript:select_area('#009900')">
<area shape="rect" coords="16,91,30,105" href="javascript:select_area('#009933')">
<area shape="rect" coords="31,91,45,105" href="javascript:select_area('#009966')">
<area shape="rect" coords="46,91,60,105" href="javascript:select_area('#009999')">
<area shape="rect" coords="61,91,75,105" href="javascript:select_area('#0099CC')">
<area shape="rect" coords="76,91,90,105" href="javascript:select_area('#0099FF')">

<area shape="rect" coords="91,91,105,105" href="javascript:select_area('#00CC00')">
<area shape="rect" coords="106,91,120,105" href="javascript:select_area('#00CC33')">
<area shape="rect" coords="121,91,135,105" href="javascript:select_area('#00CC66')">
<area shape="rect" coords="136,91,150,105" href="javascript:select_area('#00CC99')">
<area shape="rect" coords="151,91,165,105" href="javascript:select_area('#00CCCC')">
<area shape="rect" coords="166,91,180,105" href="javascript:select_area('#00CCFF')">

<area shape="rect" coords="181,91,195,105" href="javascript:select_area('#00FF00')">
<area shape="rect" coords="196,91,210,105" href="javascript:select_area('#00FF33')">
<area shape="rect" coords="211,91,225,105" href="javascript:select_area('#00FF66')">
<area shape="rect" coords="226,91,240,105" href="javascript:select_area('#00FF99')">
<area shape="rect" coords="241,91,255,105" href="javascript:select_area('#00FFCC')">
<area shape="rect" coords="256,91,270,105" href="javascript:select_area('#00FFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,106,15,120" href="javascript:select_area('#339900')">
<area shape="rect" coords="16,106,30,120" href="javascript:select_area('#339933')">
<area shape="rect" coords="31,106,45,120" href="javascript:select_area('#339966')">
<area shape="rect" coords="46,106,60,120" href="javascript:select_area('#339999')">
<area shape="rect" coords="61,106,75,120" href="javascript:select_area('#3399CC')">
<area shape="rect" coords="76,106,90,120" href="javascript:select_area('#3399FF')">

<area shape="rect" coords="91,106,105,120" href="javascript:select_area('#33CC00')">
<area shape="rect" coords="106,106,120,120" href="javascript:select_area('#33CC33')">
<area shape="rect" coords="121,106,135,120" href="javascript:select_area('#33CC66')">
<area shape="rect" coords="136,106,150,120" href="javascript:select_area('#33CC99')">
<area shape="rect" coords="151,106,165,120" href="javascript:select_area('#33CCCC')">
<area shape="rect" coords="166,106,180,120" href="javascript:select_area('#33CCFF')">

<area shape="rect" coords="181,106,195,120" href="javascript:select_area('#33FF00')">
<area shape="rect" coords="196,106,210,120" href="javascript:select_area('#33FF33')">
<area shape="rect" coords="211,106,225,120" href="javascript:select_area('#33FF66')">
<area shape="rect" coords="226,106,240,120" href="javascript:select_area('#33FF99')">
<area shape="rect" coords="241,106,255,120" href="javascript:select_area('#33FFCC')">
<area shape="rect" coords="256,106,270,120" href="javascript:select_area('#33FFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,121,15,135" href="javascript:select_area('#669900')">
<area shape="rect" coords="16,121,30,135" href="javascript:select_area('#669933')">
<area shape="rect" coords="31,121,45,135" href="javascript:select_area('#669966')">
<area shape="rect" coords="46,121,60,135" href="javascript:select_area('#669999')">
<area shape="rect" coords="61,121,75,135" href="javascript:select_area('#6699CC')">
<area shape="rect" coords="76,121,90,135" href="javascript:select_area('#6699FF')">

<area shape="rect" coords="91,121,105,135" href="javascript:select_area('#66CC00')">
<area shape="rect" coords="106,121,120,135" href="javascript:select_area('#66CC33')">
<area shape="rect" coords="121,121,135,135" href="javascript:select_area('#66CC66')">
<area shape="rect" coords="136,121,150,135" href="javascript:select_area('#66CC99')">
<area shape="rect" coords="151,121,165,135" href="javascript:select_area('#66CCCC')">
<area shape="rect" coords="166,121,180,135" href="javascript:select_area('#66CCFF')">

<area shape="rect" coords="181,121,195,135" href="javascript:select_area('#66FF00')">
<area shape="rect" coords="196,121,210,135" href="javascript:select_area('#66FF33')">
<area shape="rect" coords="211,121,225,135" href="javascript:select_area('#66FF66')">
<area shape="rect" coords="226,121,240,135" href="javascript:select_area('#66FF99')">
<area shape="rect" coords="241,121,255,135" href="javascript:select_area('#66FFCC')">
<area shape="rect" coords="256,121,270,135" href="javascript:select_area('#66FFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,136,15,150" href="javascript:select_area('#999900')">
<area shape="rect" coords="16,136,30,150" href="javascript:select_area('#999933')">
<area shape="rect" coords="31,136,45,150" href="javascript:select_area('#999966')">
<area shape="rect" coords="46,136,60,150" href="javascript:select_area('#999999')">
<area shape="rect" coords="61,136,75,150" href="javascript:select_area('#9999CC')">
<area shape="rect" coords="76,136,90,150" href="javascript:select_area('#9999FF')">

<area shape="rect" coords="91,136,105,150" href="javascript:select_area('#99CC00')">
<area shape="rect" coords="106,136,120,150" href="javascript:select_area('#99CC33')">
<area shape="rect" coords="121,136,135,150" href="javascript:select_area('#99CC66')">
<area shape="rect" coords="136,136,150,150" href="javascript:select_area('#99CC99')">
<area shape="rect" coords="151,136,165,150" href="javascript:select_area('#99CCCC')">
<area shape="rect" coords="166,136,180,150" href="javascript:select_area('#99CCFF')">

<area shape="rect" coords="181,136,195,150" href="javascript:select_area('#99FF00')">
<area shape="rect" coords="196,136,210,150" href="javascript:select_area('#99FF33')">
<area shape="rect" coords="211,136,225,150" href="javascript:select_area('#99FF66')">
<area shape="rect" coords="226,136,240,150" href="javascript:select_area('#99FF99')">
<area shape="rect" coords="241,136,255,150" href="javascript:select_area('#99FFCC')">
<area shape="rect" coords="256,136,270,150" href="javascript:select_area('#99FFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,151,15,165" href="javascript:select_area('#CC9900')">
<area shape="rect" coords="16,151,30,165" href="javascript:select_area('#CC9933')">
<area shape="rect" coords="31,151,45,165" href="javascript:select_area('#CC9966')">
<area shape="rect" coords="46,151,60,165" href="javascript:select_area('#CC9999')">
<area shape="rect" coords="61,151,75,165" href="javascript:select_area('#CC99CC')">
<area shape="rect" coords="76,151,90,165" href="javascript:select_area('#CC99FF')">

<area shape="rect" coords="91,151,105,165" href="javascript:select_area('#CCCC00')">
<area shape="rect" coords="106,151,120,165" href="javascript:select_area('#CCCC33')">
<area shape="rect" coords="121,151,135,165" href="javascript:select_area('#CCCC66')">
<area shape="rect" coords="136,151,150,165" href="javascript:select_area('#CCCC99')">
<area shape="rect" coords="151,151,165,165" href="javascript:select_area('#CCCCCC')">
<area shape="rect" coords="166,151,180,165" href="javascript:select_area('#CCCCFF')">

<area shape="rect" coords="181,151,195,165" href="javascript:select_area('#CCFF00')">
<area shape="rect" coords="196,151,210,165" href="javascript:select_area('#CCFF33')">
<area shape="rect" coords="211,151,225,165" href="javascript:select_area('#CCFF66')">
<area shape="rect" coords="226,151,240,165" href="javascript:select_area('#CCFF99')">
<area shape="rect" coords="241,151,255,165" href="javascript:select_area('#CCFFCC')">
<area shape="rect" coords="256,151,270,165" href="javascript:select_area('#CCFFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,166,15,180" href="javascript:select_area('#FF9900')">
<area shape="rect" coords="16,166,30,180" href="javascript:select_area('#FF9933')">
<area shape="rect" coords="31,166,45,180" href="javascript:select_area('#FF9966')">
<area shape="rect" coords="46,166,60,180" href="javascript:select_area('#FF9999')">
<area shape="rect" coords="61,166,75,180" href="javascript:select_area('#FF99CC')">
<area shape="rect" coords="76,166,90,180" href="javascript:select_area('#FF99FF')">

<area shape="rect" coords="91,166,105,180" href="javascript:select_area('#FFCC00')">
<area shape="rect" coords="106,166,120,180" href="javascript:select_area('#FFCC33')">
<area shape="rect" coords="121,166,135,180" href="javascript:select_area('#FFCC66')">
<area shape="rect" coords="136,166,150,180" href="javascript:select_area('#FFCC99')">
<area shape="rect" coords="151,166,165,180" href="javascript:select_area('#FFCCCC')">
<area shape="rect" coords="166,166,180,180" href="javascript:select_area('#FFCCFF')">

<area shape="rect" coords="181,166,195,180" href="javascript:select_area('#FFFF00')">
<area shape="rect" coords="196,166,210,180" href="javascript:select_area('#FFFF33')">
<area shape="rect" coords="211,166,225,180" href="javascript:select_area('#FFFF66')">
<area shape="rect" coords="226,166,240,180" href="javascript:select_area('#FFFF99')">
<area shape="rect" coords="241,166,255,180" href="javascript:select_area('#FFFFCC')">
<area shape="rect" coords="256,166,270,180" href="javascript:select_area('#FFFFFF')">

<!-- 区切 -->

<area shape="rect" coords="1,181,30,195" href="javascript:select_area('#000000')">
<area shape="rect" coords="31,181,60,195" href="javascript:select_area('#333333')">
<area shape="rect" coords="61,181,90,195" href="javascript:select_area('#666666')">

<area shape="rect" coords="91,181,120,195" href="javascript:select_area('#999999')">
<area shape="rect" coords="121,181,150,195" href="javascript:select_area('#AAAAAA')">
<area shape="rect" coords="151,181,180,195" href="javascript:select_area('#CCCCCC')">

<area shape="rect" coords="181,181,210,195" href="javascript:select_area('#DDDDDD')">
<area shape="rect" coords="211,181,240,195" href="javascript:select_area('#EEEEEE')">
<area shape="rect" coords="241,181,270,195" href="javascript:select_area('#FFFFFF')">

</map>
<img src="/?chart?/" width="271" height="196" alt="カラーチャート" usemap="#color_map">
</td>

<!-- カラーチャート終点 -->



<!-- 表示部分始点 -->
<td rowspan="2" valign="top" class="box3" height="280">
<iframe width="100%" height="300" name="ifr"></iframe>
</td>
<!-- 表示部分終点 -->
</tr>
<tr>
<td valign="top" width="271" class="box3">
<small style="font-size:10px;">
使用法Ａ）.パレットで変更したい色の項目を選択し、カラーチャートをクリックすると、自動で確認表示されます。<br>
使用法Ｂ）.色記述形式が解る方は、パレットに手動入力する事もできます。その場合、手動確認ボタンで確認できます。<br>
※<font class="stc">この機能はJavaScriptを使用します。</font>JavaScpriptを無効にしている方は、有効にしてお使い下さい。
</small>
</td>
</tr>

<tr>
	<td class="box2" width="100%" colspan="2">
	▼上記の内容で宜しければボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%" colspan="2">
	<input type="submit" value="　設定変更　"><input type="reset" value="　リセット　" onClick="default_color(1);">
	</td>
</tr>
<!-- ！パレット終点 -->
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="display"-->




<!--▼フッタブロック-->
<!--BLOCK="footer"-->
</center>
<br>
<p align=right>
<!-- ※下記は変更しないで下さい。 -->
/?version?/　/?copyright?/&nbsp; 
</p>
</body>
</html>
<!--/BLOCK="footer"--> 




