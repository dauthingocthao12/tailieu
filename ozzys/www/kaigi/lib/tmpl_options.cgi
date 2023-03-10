

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
	<td class="box2">[<a href="?">▲記事リストへ</a>] &gt; <a href="?m=/?admin_key?/&">管理用</a> &gt; <u>オプション設定</u></td>
</tr>
</table>
<!-- テーブル終点 -->
<br>
<!--/BLOCK="header"-->




<!--▼フォームブロック-->
<!--BLOCK="options"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="options2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">▼カウンタ設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<small>カウント数は「管理用 &gt; データ初期化」で操作できます。</small>
	</td>
</tr
<tr>
	<td class="box3" width="100%">
	<b class="c1">カウンタを使いますか？</b><br>
	<select name="counter">
	<option value="1"/?counter.1?/>○使う</option>
	<option value=""/?counter.?/>×使わない</option>
	</select>
	<br><small>（カウンタは記事リストの右下に表示されます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">カウンタの桁数</b><br>
	<select name="counter_fig">
	<option value="4"/?counter_fig.4?/>4ケタ</option>
	<option value="5"/?counter_fig.5?/>5ケタ</option>
	<option value="6"/?counter_fig.6?/>6ケタ</option>
	<option value="7"/?counter_fig.7?/>7ケタ</option>
	<option value="8"/?counter_fig.8?/>8ケタ</option>
	<option value="10"/?counter_fig.10?/>10ケタ</option>
	</select>
	<br><small>（カウンタの最短桁数です。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">再カウント</b><br>
	<select name="counter_up">
	<option value="0"/?counter_up.0?/>直前IPの再カウント無効</option>
	<option value="1"/?counter_up.1?/>全てカウント</option>
	</select>
	<br><small>（リロード時やページ移動時の再カウントの設定です。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼BGM設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">BGMを使いますか？</b><br>
	<select name="bgm">
	<option value="1"/?bgm.1?/>○使う</option>
	<option value=""/?bgm.?/>×使わない</option>
	</select>
	<br><small>（BGMを使う場合、下記の"BGMファイル"も設定してください。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">BGMファイル</b><br>
	<input type="text" size=50 name="bgm_src" value="/?bgm_src?/">
	<br><small>（"URL"または、ブラウザでアクセス可能な"相対パス"でMIDIファイルなど指定できます。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">BGMループ再生</b><br>
	<select name="bgm_loop">
	<option value="true"/?bgm_loop.true?/>ループ再生オン</option>
	<option value="false"/?bgm_loop.false?/>ループ再生オフ</option>
	</select>
	<br><small>（BGMのループ、"繰返し再生"です。）</small>
	</td>
</tr>

<tr>
	<td class="box2" width="100%">▼メール通知設定</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<small class="stc">※この機能は、Sendmailが設置されているサーバ、または同等のインターフェースを持つメール送信プログラムが設置されているサーバでのみ使えます。</small><small>また、文字コードはUTF-8扱いです。</small>
	</td>
</tr
<tr>
	<td class="box3" width="100%">
	<b class="c1">メール通知を使いますか？</b><br>
	<select name="mailto_admin">
	<option value="1"/?mailto_admin.1?/>○使う</option>
	<option value="0"/?mailto_admin.0?/>×使わない</option>
	</select>
	<br><small>（投稿記事を管理者"ご自分"宛にメール通知する機能です。メール通知を使う場合、下記の"メール送信プログラムのパス"と"宛先メールアドレス"、"メールの件名"も設定してください。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">Sendmailのパス</b><br>
	<input type="text" size=50 name="sendmail" value="/?sendmail?/">
	<br><small>（サーバに設置されたSendmailのパス。または同等のインターフェースを持つメール送信プログラムのパス。Sendmailが設置されているサーバでは、"<b>/usr/sbin/sendmail</b>"または、"<b>/usr/lib/sendmail</b>"がよく使われる絶対パスです。分からない場合、サーバ管理者に聞いて下さい。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">宛先メールアドレス</b><br>
	<input type="text" size=50 name="mail_addr" value="/?mail_addr?/">
	<br><small>（管理者"ご自分"のメールアドレスです。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">メールの件名</b><br>
	<input type="text" size=50 name="mail_subj" value="/?mail_subj?/">
	<br><small>（</small><small class="stc">※半角英数で記入して下さい。</small><small>例：「from web bbs」など。省略時は、"a new message"を代用します。このメール通知機能は簡易的なものであり、日本語の件名は使えません。）</small>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">メールの重要度</b><br>
	<select name="mail_priority">
	<option value="1"/?mail_priority.1?/>最高</option>
	<option value="2"/?mail_priority.2?/>高い</option>
	<option value="3"/?mail_priority.3?/>普通</option>
	<option value="4"/?mail_priority.4?/>低い</option>
	<option value="5"/?mail_priority.5?/>最低</option>
	</select>
	<br><small>（重要度に対応しているメールソフトで有効です。）</small>
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
<!--/BLOCK="options"-->




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




