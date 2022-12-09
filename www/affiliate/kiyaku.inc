<?PHP
//	規約表示
function kiyaku() {


	$title = "ネイバーズスポーツアフィリエイト(Affiliate)規約";

	//	規約読み込み
	$k_file = "./affiliate/kiyaku.txt";
	if (file_exists($k_file) && filesize($k_file) > 0) {
		$LIST = file($k_file);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$kiyaku .= $VAL;
			}
		}
	}

	$html = <<<WAKABA
      <table width="750px">
        <TBODY>
          <TR>
            <th class="cate2" align="right">
<a href="/affiliate.php">アフィリエイトTOP</a>
            </th>
          </TR>
        </TBODY>
      </TABLE>
      <table width="750px">
          <TR>
            <th class="cate2">ネイバーズスポーツアフィリエイト規約</th>
          </TR>
          <TR>
            <TD>
$kiyaku
            </TD>
          </TR>
      </TABLE>
      <BR>

WAKABA;

	return array($title,$html);

}
?>
