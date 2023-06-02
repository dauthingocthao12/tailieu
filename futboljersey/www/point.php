<?PHP
	include "./sub/menu_p.inc";
	include "./sub/cago.inc";
	include "../cone.inc";
	include "./sub/head.inc";
	include "./sub/foot.inc";
	include "./sub/array.inc";

	session_start();
	$_SESSION["idpass"];
	$idpass = $_SESSION['idpass'];

	$menu = menu($dir);

	$headmsg = headmsg();
	$footmsg = footmsg();

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub/";
	include ("$SUB_DIR/base.php");
	$title .= "ポイント確認 サッカーショップ サッカーユニフォーム ネイバーズスポーツ";

	$html .= head_html($title);
	$html .= head_menu_html();
	$html .= head_login_html();
	$html .= special_html();
	$html .= side_menu_html();

	$html .= <<<WAKABA
<div class="con_name"><div class="con_text"><B>ポイント確認</B></div></div>
WAKABA;

	list($email,$pass) = explode("<>",$idpass);

	$sql =  "select point from kojin" .
			" where email='$email' AND pass='$pass' AND kojin_num<='100001' AND saku='0'" .
			" ORDER BY kojin_num;";
	$sql1 = mysqli_query($conn_id,$sql);
	$count = mysqli_num_rows($sql1);

	if ($count >= 1) {
		list($point) = mysqli_fetch_array($sql1);
	}

	$point = number_format($point);

	$html .= <<<WAKABA
<!-- コンテンツ -->
      <table width="750px">
          <TR>
            <th class="cate2">
            　保有割引ポイントは、$point pt ( $point 円 )です。 <BR>
            </th>
          </TR>
      </TABLE>
      <BR>
<!-- コンテンツ終了 -->
WAKABA;
	$html .= foot_html();

	echo("$html");

	exit();

?>
