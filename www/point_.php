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
<table width="600" border="0" cellspacing="0" cellpadding="0">
  <tbody>
    <tr>
      <td width="600" align="center" valign="top">
<!-- コンテンツ -->
      <TABLE border="0" width="600" cellspacing="1" bgcolor="#666600" height="20">
        <TBODY>
          <TR bgcolor="#cecf63">
            <TD align="center"><B>ポイント確認</B></TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	list($email,$pass) = explode("<>",$idpass);

	$sql =  "select point from kojin" .
			" where email='$email' AND pass='$pass' AND kojin_num<='100001' AND saku='0'" .
			" ORDER BY kojin_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count >= 1) {
		list($point) = pg_fetch_array($sql1,0);
	}

	$point = number_format($point);

	$html .= <<<WAKABA
      <TABLE border="0" width="600" cellspacing="1" bgcolor="#666600">
        <TBODY>
          <TR>
            <TD background="image/back_1.gif" height="20">
            <BR>
            　保有割引ポイントは、$point pt ( $point 円 )です。 <BR>
            <BR>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
<!-- コンテンツ終了 -->
      </td>
    </tr>
  </tbody>
</table>
WAKABA;
	$html .= foot_html();
	$html .= <<<WAKABA
</html>

WAKABA;

	echo("$html");

	exit();

?>
