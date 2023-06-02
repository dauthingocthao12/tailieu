<?PHP
//	アフィリエイトコントロールプログラム
//	デフォルト
function defaults($mode,$action,$method) {
global $AF_DIR;

	$title = "ネイバーズスポーツ アフィリエイト(Affiliate)";

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	}
	elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	if (!$idpass) {			//	未会員
		include("$AF_DIR/normal.inc");
		$html = normal();
	}
	elseif ($af_num > 0) {	//	アフィリエイト会員
		include("$AF_DIR/normal_am.inc");
		$html = normal_am($af_num);
	}
	else {					//	会員
		include("$AF_DIR/normal_m.inc");
		$html = normal_m();
	}

	return array($title,$html);

}



//	登録
function regist($mode,$action,$method) {
global $AF_DIR;

	include("$AF_DIR/regist.inc");

	$title = "ネイバーズスポーツ アフィリエイト参加登録(Affiliate)";

	//	分岐
	switch ($method) {
		case "post":	//	処理
			switch ($action) {
				case "regist":	//	登録
					list($title,$main) = af_regist($mode,$action,$method);
					break;
			}
			break;

		case "get":		//	表示
			switch ($action) {
				case "end":	//	完了
					$main = af_re_end($mode,$method);
					break;

				default:		//	登録ページ初期画面
					$main = re_defaults($mode,$action,$method);
			}
			break;
	}

	if (!$main) {	//	登録不可メッセージ
		$main = not_regist($mode,$action,$method);
	}

	$html .= $main;

	return array($title,$html);

}



//	ポイント確認
function p_check($mode,$action,$method) {
global $AF_DIR;

	include("$AF_DIR/p_check.inc");

	$title = "ネイバーズスポーツ アフィリエイトポイント詳細";

	if ($_SESSION['idpass']) {
		$idpass = $_SESSION['idpass'];
	}
	elseif ($_COOKIE['idpass']) {
		$idpass = $_COOKIE['idpass'];
	}
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	//	分岐
	switch ($action) {
		case "mon":	//	ポイント詳細（月毎）
			$main = pc_mon($mode,$action,$method,$af_num);
			break;

		case "day":	//	ポイント詳細（日毎）
			$main = pc_day($mode,$action,$method,$af_num);
			break;

		default:		//	ポイント詳細（年毎）
			$main = pc_defaults($mode,$action,$method,$af_num);
	}

	$html = <<<WAKABA
$main
<br>
<form action="$PHP_SELF" method="POST">
<input type="submit" value="アフィリエイトTOPに戻る">
</form>
WAKABA;

	return array($title,$html);

}



//	ポイント変換申請
function p_appli($mode,$action,$method) {
global $PHP_SELF,$AF_DIR,$CHENGE_TYPE_L,$DEPOSIT_L;

	include("$AF_DIR/p_appli.inc");

	$title = "ネイバーズスポーツ アフィリエイトポイント変換申請";

	$idpass = $_SESSION['idpass'];
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);

	//	分岐
	switch ($method) {
		case "post":	//	処理
			switch ($action) {
				case "check":	//	確認
					p_pap_check($mode,$action,$af_num);
					break;

				case "regist":	//	登録
					p_pap_end($mode,$action,$af_num);
					break;
				default:		//	申請ページ
					$main = pap_defaults($mode,$action,$af_num);
			}
			break;

		case "get":		//	表示
			switch ($action) {
				case "check":	//	確認ページ
					$main = pap_check($mode,$action,$af_num);
					break;

				case "end":	//	手続き完了ページ
					$main = pap_end($mode,$action,$af_num);
					break;
				default:		//	申請ページ
					$main = pap_defaults($mode,$action,$af_num);
			}
			break;

		default:		//	申請ページ
			$main = pap_defaults($mode,$action,$af_num);
	}

	$html = <<<WAKABA
$main
<br>
<form action="$PHP_SELF" method="POST">
<input type="submit" value="アフェリエイトTOPに戻る">
</form>
WAKABA;

	return array($title,$html);

}



//	ポイント変換履歴・変更
function p_appli_change($mode,$action,$method) {
global $PHP_SELF,$AF_DIR,$CHENGE_TYPE_L,$DEPOSIT_L;

	include("$AF_DIR/p_appli_change.inc");

	$title = "ネイバーズスポーツ アフィリエイトポイント変換履歴";

	$idpass = $_SESSION['idpass'];
	list($email,$pass,$check,$af_num)  = explode("<>",$idpass);
	//	分岐
	switch ($method) {
		case "post":	//	処理
			switch ($action) {
				case "change":	//	詳細データー読み込み
					p_pac_change($mode,$action,$af_num);
					break;

				case "check":	//	確認
					p_pac_check($mode,$action,$af_num);
					break;

				case "regist":	//	更新処理
					p_pac_end($mode,$action,$af_num);
					break;

				case "cancel":	//	キャンセル処理
					$main = p_pac_cancel($mode,$action,$af_num);
					break;

				default:		//	履歴一覧
					$main = pac_defaults($mode,$action,$af_num);
			}
			break;

		case "get":		//	表示
			switch ($action) {
				case "change":	//	申請変更ページ
					$main = pac_change($mode,$action,$af_num);
					break;

				case "check":	//	修正確認ページ
					$main = pac_check($mode,$action,$af_num);
					break;

				case "cancel":	//	キャンセル確認ページ
					$main = pac_cancel($mode,$action,$af_num);
					break;

				case "end":	//	手続き完了ページ
					$main = pac_end($mode,$action,$af_num);
					break;

				default:		//	履歴一覧
					$main = pac_defaults($mode,$action,$af_num);
			}
			break;

		default:		//	履歴一覧
			$main = pac_defaults($mode,$action,$af_num);
	}

	$html = <<<WAKABA
$main
<br>
<form action="$PHP_SELF" method="POST">
<input type="submit" value="アフィリエイトTOPに戻る">
</form>
WAKABA;

	return array($title,$html);

}



//	解約
function cansel($mode,$action,$method) {
global $AF_DIR;






	return array($title,$html);

}



//	アフィリエイトヘッダー
function afhead() {

	$html = <<<WAKABA
      <table width="750px">
          <TR>
            <th class="cate2"><FONT size="+2">ネイバーズスポーツ アフィリエイト</FONT></th>
          </TR>
      </TABLE>
      <BR>

WAKABA;

	return $html;

}
?>
