<?PHP
//	デフォルト　未会員
function normal() {
global $aff_ritsu;

	$html = <<<WAKABA
	
<div class="con_name">
	<div class="con_text">ネイバーズスポーツ アフィリエイト</div>
</div>

<table>
	<tr>
		<th class="cate2">
		ネイバーズスポーツアフィリエイトとは？
		</th>
	</tr>
	<tr>
		<td class="cate3">
	<div id="cate3in">
ネイバーズスポーツ（当サイト：https://www.futboljersey.com/)で販売している商品を選びあなたのサイトやメルマガなどで紹介して下さい。
あなたの紹介から商品が購入されますと、商品代金の{$aff_ritsu}%のアフィリエイトポイントを差し上げます。アフィリエイトポイントは、割引ポイントや、口座にお振り込みさせて頂きます。
	</div>
		</td>
	</tr>
	<tr>
		<th class="cate2">
		アフィリエイトに参加するには？
		</th>
	</tr>
	<tr>
		<td class="cate3">
	<div id="cate3in">
		まずは、ネイバーズスポーツの会員登録をして下さい。
		詳しくはこちら又は、以下にある「会員登録する」ボタンを押しご確認・登録お願いします。
		登録後、ログインして頂きアフィリエイトのページをご覧頂くとアフィリエイト会員登録が出来ますのでご登録して下さい。
	</div>
		</td>
	</tr>
	<tr>
	<td class="cate3" align="center">
			<form action="/member/" method="post">
            <input type="submit" value="会員登録する">
	</td></form>
	</tr>
</table>	
      <BR>
      <p><a href="/affiliate.php?mode=kiyaku">アフィリエイト規約</a></p>

WAKABA;

	return $html;

}
?>
