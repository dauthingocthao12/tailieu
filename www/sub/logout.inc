<?PHP
//	ログアウト
function logout() {

	if ($_GET['url']) {
		$burl = $_GET['url'];
	}
	$idpass = $_SESSION['idpass'];

	$html = first($burl);

	return $html;

}



//	ログアウトページ
function first($burl) {

	$html = <<<WAKABA
      <BR>$idpass
      <TABLE border="0" width="95%" bgcolor="#000000" cellpadding="4">
        <TBODY>
          <TR>
            <TH><FONT color="#ffffff" size="+1">ログアウト</FONT></TH>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
      <TABLE border="0" width="95%" cellspacing="0" cellpadding="0">
        <TBODY>
          <TR>
            <TD align="center">
            <TABLE border="0" width="400">
              <TBODY>
                <TR>
                  <TD>ログアウトしました。$burl</TD>
                </TR>
              </TBODY>
            </TABLE>
            </TD>
          </TR>
        </TBODY>
      </TABLE>

WAKABA;

	unset($idpass);
	unset($_SESSION['idpass']);
	setcookie("idpass");

	return $html;

}
?>
