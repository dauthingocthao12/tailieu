<?PHP
//	デフォルト　会員
function normal_m() {
global $PHP_SELF,$aff_ritsu;

	$html = <<<WAKABA
      <table width="750px">
          <TR>
            <th class="cate2">ネイバーズスポーツアフィリエイトに参加しませんか？？</th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>
            <BLOCKQUOTE><BR>
ネイバーズスポーツ（当サイト：https://www.futboljersey.com/)で販売している商品を選びあなたのサイトやメルマガなどで紹介して下さい。<br>
あなたの紹介から商品が購入されますと、商品代金の{$aff_ritsu}%のアフィリエイトポイントを差し上げます。アフィリエイトポイントは、割引ポイントや、口座にお振り込みさせて頂きます。<br>
            </BLOCKQUOTE>
            </TD>
          </TR>
          <TR>
            <th class="cate2">まずは登録！</th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>
            <BLOCKQUOTE><BR>
以下にある「アフィリエイトに参加する」ボタンを押し、規約を読み承諾して頂ければご登録は完了です。<br>
各ページに下方に、あなた専用のアフィリエイトURLが表示されますので、あなたのサイトやメルマガなどに張って頂ければ開始です。<br>
            </BLOCKQUOTE>
            </TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD align="center"><form action="$PHP_SELF" method="get">
            <input type="hidden" name="mode" value="regist">
            <input type="submit" value="アフィリエイトに参加する">
            </TD></form>
          </TR>
      </TABLE>
      <BR>
      <p><a href="/affiliate.php?mode=kiyaku">アフィリエイト規約</a></p>

WAKABA;

	return $html;

}
?>
