<?PHP
//	ヘッドメッセージ

	include("./sub/setup.inc");
	include("./sub/headmsg.inc");

	$headmsg = headmsg($LOGDATA_DIR,$headmsg_file);

	echo($headmsg);

	exit;

?>
