<?PHP

function menu0($login,$m_list) {
global $index,$dir;

	unset($index);

	$menu_html_file = "./data/cat_menu.inc";
	if ($dir == 1) {
		$menu_html_file = "../data/cat_menu.inc";
	}
	$menu_html = file_get_contents($menu_html_file);

	$menu = <<<WAKABA
<div id="navbox" class="navbox">
<form action="/goodssearch.php" method="GET">
<table width="150" border="0" cellpadding="0" cellspacing="0">
<tr align="center">
<td>
<!-- <img src="/image/200805s.jpg" alt="サッカーショップ
ネイバーズスポーツ NBS
サッカースパイク
サッカーユニフォーム
サッカー用品
スペシャルバザール
30%OFF" width="150" height="100" border="0">
<img src="/image/20080529s.jpg" alt="サッカーショップ
ネイバーズスポーツ NBS
サッカースパイク
サッカーユニフォーム
サッカー用品
四日間限定半額セール" width="150" height="100" border="0"> -->
<br>
</td>
</tr>
<tr align="center">
<td>
<a href="/goodssearch.php" title="サッカー用品
ユニフォーム
スパイク
商品検索"><img src="/image/search_title.gif" alt="商品検索" width="150" height="28" border="0"></a></td>
</tr>
<tr align="center">
<td background="/image/search_back.gif">
<input size="18" type="text" name="word"><br />
<input type="submit" value="商品検索">
</td>
</tr>
<tr align="center">
<td><img src="/image/search_foot.gif" width="150" height="8" border="0"></td>
</tr>
</table>
</form>
$login
<br />

<!--左メニューのカテゴリーを表示させている部分です//-->
<a href="/goods/" title="カテゴリー"><img src="/image/menu_title.gif" width="150" height="30" border="0" alt="カテゴリー"></a><br />
$menu_html
<!--カテゴリーメニューここまで//-->
      <div class="line_height30">
      <a href="s/marking.php" class="l_marking" title="サッカーユニフォーム
マーキング">サッカーユニフォーム
マーキング</a><br>
      <a href="/1-7/$index" class="l_howtobuy" title="商品購入方法">商品購入方法</a><br>
      <a href="/cautions.htm" class="l_cautions" title="問題解決方法">問題解決方法</a><br>
      <a href="/team.php" class="l_team" title="サッカーユニフォーム
チームオーダー">サッカーユニフォーム
チームオーダー</a><br>
      <a href="/fax/$index" class="l_fax" title="FAX注文">FAX注文</a><br>
      <a href="/affiliate.php" class="l_affiliate" title="アフィリエイト">アフィリエイト</a><br>
$m_list
      <a href="/privacypolicy.htm" class="l_privacy" title="プライバシー">プライバシー</a><br>
      <a href="/1-8/$index" class="l_link" title="リンク">リンク</a><br>
      </div>
      <br>

<table border="0" width="150" cellspacing="1" cellpadding="0" bgcolor="#116998">
  <tbody>
    <tr bgcolor="#d3ecfa">
      <th><font color="#5d7c00">特別相互リンク</font></th>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td><a href="http://www.ozzys.jp/" target="_blank" title="ルアー＆フライフィッシング オンラインショップ Ozzy's">ルアー＆フライフィッシング オンラインショップ オジーズ</a></td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td><a href="http://www.seotools.jp/" target="_blank" title="SEOアクセス解析ツール">SEOアクセス解析ツール</a></td>
    </tr>
  </tbody>
</table>

      <br>
      <br>
      </div>

WAKABA;

	return $menu;

}
?>
