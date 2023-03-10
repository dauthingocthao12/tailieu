

<!--■ 管理用テンプレートファイル -->
<!--■ 管理用なので、改造しない方が無難です。 -->




<!--▼ヘッダブロック-->
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
</head>
<body><center>
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
	<td class="box2">[<a href="?">▲記事リストへ</a>] &gt; <a href="?m=/?admin_key?/&">管理用</a> &gt; <u>詳細設定</u></td>
</tr>
</table>
<!-- テーブル終点 -->
<br>
<!--/BLOCK="header"-->




<!--▼フォームブロック-->
<!--BLOCK="detail"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="detail2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">▼記事ファイル設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">話題の最大記録数</b><br>
	<select name="max_recs">
	<option value="10"/?max_recs.10?/>10件</option>
	<option value="20"/?max_recs.20?/>20件</option>
	<option value="30"/?max_recs.30?/>30件</option>
	<option value="40"/?max_recs.40?/>40件</option>
	<option value="50"/?max_recs.50?/>50件</option>
	<option value="75"/?max_recs.75?/>75件</option>
	<option value="100"/?max_recs.100?/>100件</option>
	<option value="200"/?max_recs.200?/>200件</option>
	<option value="300"/?max_recs.300?/>300件</option>
	<option value="500"/?max_recs.500?/>500件</option>
	<option value="1000"/?max_recs.1000?/>1000件</option>
	</select>
	<br><small>（記録できる話題の最大記事数です。話題は古い物から順に削除されます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">ページあたり最大話題表示数</b><br>
	<select name="page_recs">
	<option value="4"/?page_recs.4?/>4件</option>
	<option value="5"/?page_recs.5?/>5件</option>
	<option value="6"/?page_recs.6?/>6件</option>
	<option value="7"/?page_recs.7?/>7件</option>
	<option value="8"/?page_recs.8?/>8件</option>
	<option value="9"/?page_recs.9?/>9件</option>
	<option value="10"/?page_recs.10?/>10件</option>
	<option value="15"/?page_recs.15?/>15件</option>
	<option value="20"/?page_recs.20?/>20件</option>
	<option value="25"/?page_recs.25?/>25件</option>
	<option value="30"/?page_recs.30?/>30件</option>
	<option value="40"/?page_recs.40?/>40件</option>
	<option value="50"/?page_recs.50?/>50件</option>
	</select>
	<br><small>（１ページに表示できる最大話題数です。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">１話題あたり最大返事数</b><br>
	<select name="max_res">
	<option value="0"/?max_res.0?/>上限なし</option>
	<option value="5"/?max_res.5?/>5件</option>
	<option value="10"/?max_res.10?/>10件</option>
	<option value="15"/?max_res.15?/>15件</option>
	<option value="20"/?max_res.20?/>20件</option>
	<option value="30"/?max_res.30?/>30件</option>
	<option value="40"/?max_res.40?/>40件</option>
	<option value="50"/?max_res.50?/>50件</option>
	<option value="100"/?max_res.100?/>100件</option>
	</select>
	<br><small>（新規に投稿された話題へ、返事を投稿可能な最大件数です。例えば、"10件"で設定した場合、既に"10件"の返事が投稿されている話題に対して、返事の投稿を禁止します。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">返事の投稿後の話題と関連記事の位置</b><br>
	<select name="moveto_top">
	<option value="1"/?moveto_top.1?/>先頭に移動（※標準）</option>
	<option value="0"/?moveto_top.0?/>そのまま</option>
	</select>
	<br><small>（返事を投稿後の話題と関連記事の位置です。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼新規投稿設定（基本）</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">未記入時題名</b><br>
	<input type="text" size=50 name="std_subj" value="/?std_subj?/">
	<br><small>（新規投稿時に省略された題名の代用です。例:無題）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">未記入時名前</b><br>
	<input type="text" size=50 name="std_name" value="/?std_name?/">
	<br><small>（新規投稿時に省略された名前の代用です。例:名無し）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">題名等文字数制限</b><br>
	<select name="max_char">
	<option value="0"/?max_char.0?/>制限無し</option>
	<option value="50"/?max_char.50?/>全角25文字位まで（50B）</option>
	<option value="100"/?max_char.100?/>全角50文字位まで（100B）</option>
	<option value="150"/?max_char.150?/>全角75文字位まで（150B）</option>
	<option value="200"/?max_char.200?/>全角100文字位まで（200B）</option>
	</select>
	<br><small>（題名、名前等の文字数制限です。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">メッセージ文字数制限</b><br>
	<select name="max_msg">
	<option value="0"/?max_msg.0?/>制限無し</option>
	<option value="1000"/?max_msg.1000?/>全角500文字位まで（1,000B）</option>
	<option value="2000"/?max_msg.2000?/>全角1,000文字位まで（2,000B）</option>
	<option value="3000"/?max_msg.3000?/>全角1,500文字位まで（3,000B）</option>
	<option value="4000"/?max_msg.4000?/>全角2,000文字位まで（4,000B）</option>
	<option value="6000"/?max_msg.6000?/>全角3,000文字位まで（6,000B）</option>
	<option value="8000"/?max_msg.8000?/>全角4,000文字位まで（8,000B）</option>
	<option value="10000"/?max_msg.10000?/>全角5,000文字位まで（10,000B）</option>
	<option value="20000"/?max_msg.20000?/>全角10,000文字位まで（20,000B）</option>
	</select>
	<br><small>（メッセージの文字数制限です。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">ホスト情報取得タイプ</b><br>
	<select name="hostname">
	<option value="0"/?hostname.0?/>IPアドレス（例:127.0.0.1）</option>
	<option value="1"/?hostname.?/>ホスト名優先（例:internet-provider.ne.jp）</option>
	</select>
	<br><small>（投稿者のホスト情報です。"ホスト名優先"はサーバによって動作しない事もあります。ホスト名優先で設定した場合はテスト投稿を行ってみて下さい。設定はいつでも変更できますが、一度取得したホスト情報は変更できません。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼新規投稿設定（入力必須項目）</td>
</tr>

<tr>
	<td class="box3" width="100%">
	<b class="c1">題名の入力</b><br>
	<select name="check_subj">
	<option value="0"/?check_subj.0?/>任意（※標準）</option>
	<option value="1"/?check_subj.1?/>必須</option>
	</select>
	<br><small>（"任意"は、省略時に未記入時題名が代用されます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">名前の入力</b><br>
	<select name="check_name">
	<option value="0"/?check_name.0?/>任意（※標準）</option>
	<option value="1"/?check_name.1?/>必須</option>
	</select>
	<br><small>（"任意"は、省略時に未記入時名前が代用されます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">メールの入力</b><br>
	<select name="check_mail">
	<option value="0"/?check_mail.0?/>任意（※標準）</option>
	<option value="1"/?check_mail.1?/>必須（書式検査なし）</option>
	<option value="2"/?check_mail.2?/>必須（書式検査あり）</option>
	</select>
	<br><small>（"書式検査あり"は、メールアドレスの書式を含めて検査します。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">ＵＲＬの入力</b><br>
	<select name="check_url">
	<option value="0"/?check_url.0?/>任意（※標準）</option>
	<option value="1"/?check_url.1?/>必須</option>
	</select>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">メッセージの入力</b><br>
	<select name="check_msg">
	<option value="0"/?check_msg.0?/>任意</option>
	<option value="1"/?check_msg.1?/>必須（※標準）</option>
	</select>
	<br><small>（通常、"必須"をお勧めしています。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">スペースのみの投稿</b><br>
	<select name="check_space">
	<option value="0"/?check_space.0?/>許可する</option>
	<option value="1"/?check_space.1?/>許可しない</option>
	</select>
	<br><small>（スペースや改行のみ投稿への制限です。上記の項目の"必須"、"任意"を問わず制限できます。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼新規投稿設定（セッション管理）</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">セッション変数有効時間</b><br>
	<select name="session_timeout">
	<option value="0"/?session_timeout.0?/>セッション管理しない</option>
	<option value="10"/?session_timeout.10?/>１０分間有効</option>
	<option value="20"/?session_timeout.20?/>２０分間有効</option>
	<option value="30"/?session_timeout.30?/>３０分間有効</option>
	<option value="60"/?session_timeout.60?/>１時間有効（※標準）</option>
	<option value="180"/?session_timeout.180?/>３時間有効</option>
	<option value="360"/?session_timeout.360?/>６時間有効</option>
	<option value="720"/?session_timeout.720?/>１２時間有効</option>
	<option value="1440"/?session_timeout.1440?/>２４時間有効</option>
	<option value="2880"/?session_timeout.2880?/>４８時間有効</option>
	</select>
	<br><small>（この設定は、二重投稿防止、連続投稿防止、不正投稿予防などの機能を提供します。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">連続投稿制限時間</b><br>
	<select name="session_interval">
	<option value="0"/?session_interval.0?/>制限無し</option>
	<option value="1"/?session_interval.1?/>１分間</option>
	<option value="2"/?session_interval.2?/>２分間</option>
	<option value="3"/?session_interval.3?/>３分間</option>
	<option value="4"/?session_interval.4?/>４分間</option>
	<option value="5"/?session_interval.5?/>５分間</option>
	<option value="10"/?session_interval.10?/>１０分間</option>
	<option value="20"/?session_interval.20?/>２０分間</option>
	<option value="30"/?session_interval.30?/>３０分間</option>
	<option value="60"/?session_interval.60?/>１時間</option>
	</select>
	<br><small>（例えば"１分間"で設定した場合、投稿完了後の１分間、その投稿者のみ連続投稿が制限されます。また、上記の"セッション変数有効時間"以下で設定して下さい。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼HTMLタグ設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">HTMLタグ</b><br>
	<select name="tag">
	<option value="0"/?tag.0?/>HTMLタグ無効（※お勧め）</option>
	<option value="1"/?tag.1?/>HTMLタグ有効</option>
	</select>
	<br><small>（HTMLタグ表示の設定です。但し、タグを有効にした場合、もしかしたらイタズラする人がいるかもしれません。セキュリティ上は"無効"をお勧めしています。いつでも変更できます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">HTMLタグNGワード</b><br>
	<input type="text" size=50 name="ng_words" value="/?ng_words?/">
	<br><small>（上記のHTMLタグで"有効"を選んだ場合に、NGワードを指定できます。NGワードは、半角英字で半角カンマ","で区切り記入して下さい。大文字小文字はどちらでも構いません。タグ内にNGワードを含む記事は、タグが無効になります。また、タグを有効にした場合、「<font class="stc">META,JavaScript,STYLE,HREF,!--</font>」を最低限指定することをお勧めします。<u>よく分からない方はHTMLタグ「無効」にする方が無難です。</u>いつでも変更できます。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼管理用設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">管理用画面フォントサイズ</b><br>
	<select name="admin_font">
	<option value="10px"/?admin_font.10px?/>10:px（小さい）</option>
	<option value="12px"/?admin_font.12px?/>12:px（小さめ）</option>
	<option value="14px"/?admin_font.14px?/>14:px（やや大きめ）</option>
	<option value="16px"/?admin_font.16px?/>16:px（大きめ）</option>
	<option value="18px"/?admin_font.18px?/>18:px（大きい）</option>
	<option value="24px"/?admin_font.24px?/>24:px（特大）</option>
	</select>
	<br><small>（今見ている管理用画面のフォントサイズです。管理しやすい大きさに設定できます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">管理用画面カラー</b><br>
	<select name="admin_theme">
	<option value="0"/?admin_theme.0?/>スタンダード</option>
	<option value="1"/?admin_theme.1?/>オレンジ</option>
	<option value="2"/?admin_theme.2?/>インディゴ</option>
	<option value="3"/?admin_theme.3?/>ピーチ</option>
	<option value="4"/?admin_theme.4?/>バイオレット</option>
	<option value="5"/?admin_theme.5?/>マロン</option>
	<option value="6"/?admin_theme.6?/>フォレスト</option>
	<option value="7"/?admin_theme.7?/>グレー</option>
	<option value="8"/?admin_theme.8?/>ストロベリー</option>
	<option value="9"/?admin_theme.9?/>ライム</option>
	<option value="10"/?admin_theme.10?/>チョコレート</option>
	<option value="11"/?admin_theme.11?/>グレープ</option>
	</select>
	<br><small>（今見ている管理用画面のテーマカラーです。管理しやすい色に設定できます。）</small>
	</td>
</tr>
<tr>
	<td class="box2" width="100%">▼上記の内容で設定変更しますか？</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<input type="submit" value="　設定変更　"><input type="reset" value="　リセット　">
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="detail"-->




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




