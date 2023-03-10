

<!--■ 管理用テンプレートファイル -->
<!--■ 管理用なので、改造しない方が無難です。 -->




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
<!--/BLOCK="header"-->




<!--▼ヘッダブロック/管理画面トップ-->
<!--BLOCK="header1"-->
<!-- テーブル始点 -->
<table width="100%" cellpadding=6 cellspacing=0 border=0>
<tr>
	<td class="box2">[<a href="?">▲記事リストへ</a>] &gt; 管理用</td>
</tr>
</table>
<!-- テーブル終点 -->
<br>
<!--/BLOCK="header1"-->




<!--▼ヘッダブロック/管理画面トップ以外-->
<!--BLOCK="header2"-->
<!-- テーブル始点 -->
<table width="100%" cellpadding=6 cellspacing=0 border=0>
<tr>
	<td class="box2">[<a href="?">▲記事リストへ</a>] &gt; <a href="?m=/?admin_key?/&">管理用</a> &gt; <u>/?sub_title?/</u></td>
</tr>
</table>
<!-- テーブル終点 -->
<br>
<!--/BLOCK="header2"-->




<!--▼メニューブロック/管理画面トップ-->
<!--BLOCK="menu"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="admin_top" value="1">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">
	▼実行モードを選択して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<select name="m2">
	<option value="readme" selected>留意事項設定</option>
	<option value="display">表示設定</option>
	<option value="detail">詳細設定</option>
	<option value="options">オプション設定</option>
	<option value="delete">レコード削除</option>
	<option value="edit">レコード再編集</option>
	<option value="restore">データ初期化</option>
	</select>
	</td>
</tr>
<tr>
	<td class="box2" width="100%">
	▼管理用パスワードを入力して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	管理用パスワード :<input type="password" size=16 name="admin_passw" value="/?admin_passw?/"> 
	保存<input type="checkbox" name="passw_ck" value="1"/?switch?/>
	</td>
</tr>
<tr>
	<td class="box2" width="100%">
	▼ボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<input type="submit" value="　実行　"><input type="reset" value="　リセット　">
	</td>
</tr>

<tr>
	<td class="box2" width="100%">
	▼実行モードについて
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">留意事項設定</b>
	<blockquote>
	<small>
	訪問者の方への留意事項を設定します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">表示設定</b>
	<blockquote>
	<small>
	プログラムの表示に関する設定。タイトル、ホームページ、タイトル画像、背景画像の設定など。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">詳細設定</b>
	<blockquote>
	<small>
	プログラムの詳細設定。管理用画面のテーマカラーを変更も。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">オプション設定</b>
	<blockquote>
	<small>
	プログラムのオプション機能の設定。カウンタ、BGM、メール通知など。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">レコード削除</b>
	<blockquote>
	<small>
	選択した記事を削除します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">レコード再編集</b>
	<blockquote>
	<small>
	選択した記事を再編集します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<b class="c1">データ初期化</b>
	<blockquote>
	<small>
	データを選択し初期化します。
	</small>
	</blockquote>
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="menu"-->




<!--▼応答ブロック-->
<!--BLOCK="response"-->
<p></p>
<form method="POST" action="?">
<input type="hidden" name="m" value="list">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">
	▼応答メッセージ
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	/?response?/
	</td>
</tr>
<tr>
	<td class="box2" width="100%">
	▼リロード用ボタン
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<input type="submit" value="　記事リストをリロード　">
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="response"-->




<!--▼留意事項設定ブロック-->
<!--BLOCK="readme"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="readme2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">
	▼全般の留意事項
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<textarea name="readme" cols="80" rows="8">/?readme?/</textarea><br>
	<small>
	<font class="stc">HTMLタグ入力可</font>。利用者の方への留意事項です。空白（改行も無し）で設定すると、留意事項は「特にありません」と表示されます。
	</small>
	</td>
</tr>
<tr>
	<td class="box2" width="100%">
	▼新規投稿フォームに表示する留意事項
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<textarea name="readme_form" cols="80" rows="8">/?readme_form?/</textarea><br>
	<small>
	<font class="stc">HTMLタグ入力可</font>。利用者の方への留意事項です。空白（改行も無し）で設定すると、何も表示しません。
	</small>
	</td>
</tr>
<tr>
	<td class="box2" width="100%">
	▼返事を投稿フォームに表示する留意事項
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
	<textarea name="readme_resform" cols="80" rows="8">/?readme_resform?/</textarea><br>
	<small>
	<font class="stc">HTMLタグ入力可</font>。利用者の方への留意事項です。空白（改行も無し）で設定すると、何も表示しません。
	</small>
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
<!--/BLOCK="readme"-->




<!--▼削除用レコードリストのヘッダブロック-->
<!--BLOCK="delete_header"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="delete2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" colspan="8">
	▼削除する話題または記事を選択して下さい。
	</td>
</tr>
<tr>
	<td class="box3" colspan="8">
	<small>"話題削除"は、その話題を完全に削除します。</small>
	</td>
</tr>
<tr>
	<td class="box3"><b class="c1">話題<br>削除</b></td>
	<td class="box3"><b class="c1">記事<br>削除</b></td>
	<td class="box3"><b class="c1">話題</b></td>
	<td class="box3"><b class="c1">番号</b></td>
	<td class="box3"><b class="c1">題名</b></td>
	<td class="box3"><b class="c1">名前</b></td>
	<td class="box3"><b class="c1">メッセージ</b></td>
	<td class="box3"><b class="c1">投稿日</b></td>
</tr>
<!--/BLOCK="delete_header"-->




<!--▼削除用レコードリストのレコードブロック-->
<!--BLOCK="delete_rec"-->
<tr>
	<td class="box3">
	<!--SUB="topic"--><input type="radio" name="delete" value="topic./?bnum?/./?num?/"><!--/SUB="topic"-->
	<!--SUB="res"-->--<!--/SUB="res"-->
	</td>
	<td class="box3">
	<!--SUB="stat1"--><input type="radio" name="delete" value="rec./?bnum?/./?num?/"><!--/SUB="stat1"-->
	<!--SUB="stat0"-->×<!--/SUB="stat0"-->
	</td>
	<td class="box3"><small>/?bnum2?/</small></td>
	<td class="box3"><small>/?num2?/</small></td>
	<td class="box3"><small>/?subj?/</small></td>
	<td class="box3"><small>/?name?/</small></td>
	<td class="box3"><small>/?msg?/</small></td>
	<td class="box3"><small>/?date?/</small></td>
</tr>
<!--/BLOCK="delete_rec"-->




<!--▼削除用レコードリストのフッタブロック-->
<!--BLOCK="delete_footer"-->
<tr>
	<td class="box2" width="100%" colspan="8">
	▼削除する話題または記事が決まったらボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%" colspan="8">
	<input type="submit" value="　削除　"><input type="reset" value="　リセット　">
	</td>
</tr>

</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="delete_footer"-->




<!--▼再編集用レコードリストのヘッダブロック-->
<!--BLOCK="edit_header"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="edit2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" colspan="7">
	▼再編集する記事を選択して下さい。
	</td>
</tr>
<tr>
	<td class="box3"><b class="c1">選択</b></td>
	<td class="box3"><b class="c1">話題</b></td>
	<td class="box3"><b class="c1">番号</b></td>
	<td class="box3"><b class="c1">題名</b></td>
	<td class="box3"><b class="c1">名前</b></td>
	<td class="box3"><b class="c1">メッセージ</b></td>
	<td class="box3"><b class="c1">投稿日</b></td>
</tr>
<!--/BLOCK="edit_header"-->




<!--▼再編集用レコードリストのレコードブロック-->
<!--BLOCK="edit_rec"-->
<tr>
	<td class="box3">
	<!--SUB="stat1"--><input type="radio" name="num" value="/?bnum?/./?num?/"><!--/SUB="stat1"-->
	<!--SUB="stat0"-->×<!--/SUB="stat0"-->
	</td>
	<td class="box3"><small>/?bnum2?/</small></td>
	<td class="box3"><small>/?num2?/</small></td>
	<td class="box3"><small>/?subj?/</small></td>
	<td class="box3"><small>/?name?/</small></td>
	<td class="box3"><small>/?msg?/</small></td>
	<td class="box3"><small>/?date?/</small></td>
</tr>
<!--/BLOCK="edit_rec"-->




<!--▼再編集用レコードリストのフッタブロック-->
<!--BLOCK="edit_footer"-->
<tr>
	<td class="box2" width="100%" colspan="7">
	▼再編集する記事が決まったらボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3" width="100%" colspan="7">
	<input type="submit" value="　再編集　"><input type="reset" value="　リセット　">
	</td>
</tr>

</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="edit_footer"-->




<!--▼レコードリストのページ移動のヘッダブロック-->
<!--BLOCK="move_header"-->
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2" width="100%">
	▼別の記事の選択する場合は移動出来ます。
	</td>
</tr>
<tr>
	<td class="box3" width="100%">
		<!-- テーブル始点 -->
		<table cellpadding=0 cellspacing=0 border=0>
		<tr>
<!--/BLOCK="move_header"-->




<!--▼レコードリストのページ移動の前ブロック-->
<!--BLOCK="prev"-->
		<td>
		<form method="POST">
		<input type="hidden" name="m" value="/?admin_key?/.2">
		<input type="hidden" name="m2" value="/?m2?/">
		<input type="hidden" name="page" value="/?prev?/">
		<input type="hidden" name="admin_passw" value="/?admin_passw?/">
		<input type="submit" value="　前のレコード　">
		</form>
		</td>
<!--/BLOCK="prev"-->




<!--▼レコードリストのページ移動の次ブロック-->
<!--BLOCK="next"-->
		<td>
		<form method="POST">
		<input type="hidden" name="m" value="/?admin_key?/.2">
		<input type="hidden" name="m2" value="/?m2?/">
		<input type="hidden" name="page" value="/?next?/">
		<input type="hidden" name="admin_passw" value="/?admin_passw?/">
		<input type="submit" value="　次のレコード　">
		</form>
		</td>
<!--/BLOCK="next"-->




<!--▼レコードリストのページ移動のフッタブロック-->
<!--BLOCK="move_footer"-->
		</tr>
		</table>
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
<!--/BLOCK="move_footer"-->




<!--▼再編集ブロック-->
<!--BLOCK="rec_edit"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="edit3">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<input type="hidden" name="bnum" value="/?bnum?/">
<input type="hidden" name="pnum" value="/?pnum?/">
<input type="hidden" name="num" value="/?num?/">
<input type="hidden" name="depth" value="/?depth?/">
<input type="hidden" name="stat" value="2">
<input type="hidden" name="date" value="/?date?/">
<input type="hidden" name="host" value="/?host?/">
<input type="hidden" name="passw" value="/?passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2">
	▼訂正箇所を編集して下さい。
	</td>
</tr>
<tr><td class="box3">
	<tt>
	<b class="c1">話題</b> : /?bnum?/番<br>
	<b class="c1">親番</b> : /?pnum?/番<br>
	<b class="c1">番号</b> : /?num?/番<br>
	<b class="c1">投稿日</b> : /?date2?/<br>
	<b class="c1">ホスト</b> : /?host?/
	</tt>
</td></tr>
<tr><td class="box3">
	<b class="c1">題名</b><br>
	<input type="text" name="subj" value="/?subj?/" size="30">
</td></tr>
<tr><td class="box3">
	<b class="c1">名前</b><br>
	<input type="text" name="name" value="/?name?/" size="30">
</td></tr>
<tr><td class="box3">
	<b class="c1">メール</b><br>
	<input type="text" name="mail" value="/?mail?/" size="30">
</td></tr>
<tr><td class="box3">
	<b class="c1">ＵＲＬ</b><br>
	<input type="text" name="url" value="/?url?/" size="30">
</td></tr>
<tr><td class="box3">
	<b class="c1">メッセージ</b><br>
	<textarea name="msg" cols="60" rows="5">/?msg?/</textarea>
</td></tr>
<tr><td class="box3">
	<b class="c1">編集キー</b><br>
	<input type="password" name="new_passw" value="" size="8" maxlength="16">
	<br><small>（<font class="stc">※編集キーを変更する場合のみ、記入して下さい。</font>編集キーは暗号化されているので表示できません。空欄で変更すれば、現在の編集キーが適用されます。）</small>
</td></tr>
<tr>
	<td class="box2">
	▼以上の内容で宜しければボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="submit" value="　編集反映　"><input type="reset" value="　リセット　">
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="rec_edit"-->




<!--▼リストアブロック-->
<!--BLOCK="restore"-->
<p></p>
<form method="POST">
<input type="hidden" name="m" value="/?admin_key?/.2">
<input type="hidden" name="m2" value="restore2">
<input type="hidden" name="admin_passw" value="/?admin_passw?/">
<!-- テーブル始点 -->
<table width="95%" cellpadding=0 cellspacing=0 border=0>
<tr>
<td class="box0">
<!-- テーブル始点 -->
<table width="100%" cellpadding=0 cellspacing=1 border=0>
<tr>
	<td class="box2">
	▼初期化するデータを選択して下さい。
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="display" value="1"> 
	<b class="c1">表示設定データ</b>
	<blockquote>
	<small>
	表示設定を設置時の内容に戻します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="detail" value="1"> 
	<b class="c1">詳細設定データ</b>
	<blockquote>
	<small>
	詳細設定を設置時の内容に戻します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="options" value="1"> 
	<b class="c1">オプション設定データ</b>
	<blockquote>
	<small>
	オプション設定を設置時の内容に戻します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="counter" value="1"> 
	<b class="c1">カウント数データ</b>
	<small>→
	初期化後のカウント数：</small><input type="text" size="12" name="new_count" value="">
	<blockquote>
	<small>
	オプション設定のカウンタで使うカウント数を初期化します。初期化後のカウント数は半角数字で記述して下さい。空欄の場合、"0"に戻ります。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="readme" value="1"> 
	<b class="c1">留意事項データ</b>
	<blockquote>
	<small>
	留意事項データを初期化します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="recs" value="1"> 
	<b class="c1">全記事データ</b>
	<blockquote>
	<small>
	全記事データを消去し、記事番号を初期化します。
	</small>
	</blockquote>
	</td>
</tr>
<tr>
	<td class="box2">
	▼初期化する前に確認して下さい。
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="checkbox" name="check" value="1"> 
	<b class="c1">初期化確認</b><br>
	<small>
	（<font style="color:#FF0000;"><u>※初期化したデータは完全に失われます。</u>よくご確認の上、「初期化確認」にチェックを入れてから実行して下さい。</font>）
	</small>
	
	</td>
</tr>
<tr>
	<td class="box2">
	▼初期化するデータが決まったらボタンを押して下さい。
	</td>
</tr>
<tr>
	<td class="box3">
	<input type="submit" value="　初期化　"><input type="reset" value="　リセット　">
	</td>
</tr>
</table>
<!-- テーブル終点 -->
</td>
</tr>
</table>
<!-- テーブル終点 -->
</form>
<!--/BLOCK="restore"-->




