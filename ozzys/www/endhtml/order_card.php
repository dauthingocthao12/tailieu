<?php

//	基本ファイル読込
include("../../cone.inc");
include("../sub/setup.inc");
include("../sub/array.inc");
include("../sub/webcollect.class.inc");
include("../sub/mail.inc");

session_start();
$ERROR = array();

// default status
$webcollect_status = WEBCOLLECT_STATUS_UNPAID;
$host = Webcollect::getHost();
$need_form = true; // default, フォーム表示（webcollectへのボタン）
$passwd_ok = isset($_SESSION['webcollect_passwd_ok']);
$show_loginform = true;
$show_recreate_cart_btn = false;

// ====================================
//print_r($_SESSION['idpass']);

// order_no データ
if($_GET['on']) {
	// メールからくるときに
	$order_no = strtoupper($_GET['on']);

    if($_SESSION['idpass']!="") {
        // ログインされてる方にはパスワードが必要ない
        $passwd_ok = true;
    }
}
else if($_SESSION['webcollect_order_no']) {
	// カートから来るときに
	$order_no = $_SESSION['webcollect_order_no'];
    $passwd_ok = true;
}
else {
	$ERROR[] = WEBCOLLECT_MSG_WEIRD;
    $show_loginform = false;
}

// ====================================

// PASSWD CHECK
if(!$passwd_ok && $order_no && $_POST['passwd']) {
    if(Webcollect::checkPasswd($order_no, $_POST['passwd'])) {
        $_SESSION['webcollect_passwd_ok'] = true;
        $passwd_ok = true;
    }
    else {
        $ERROR[] = WEBCOLLECT_MSG_PASSWD;
    }
}

// ====================================

// check order status
if(count($ERROR)==0) {
	$order = Webcollect::readOrder($order_no);
	if($order == null) {
        // order not found
		$WARNING[] = WEBCOLLECT_MSG_WEIRD;
        $show_loginform = false;
	}
}
else {
	$order = null;
}

// order customer details
if($order) {
	$order_add = Webcollect::readAdd($order['add_num']);
//	print_r($order_add);
	if($order_add == null) {
		$ERROR[] = WEBCOLLECT_MSG_DATANOTFOUND;
	}
	else {
		$name_kanji = $order_add['name_s'].'　'.$order_add['name_n'];
		$name_kana  = $order_add['kana_s'].'　'.$order_add['kana_n'];
		// $name_kana  = mb_convert_kana($name_kana, "KVC", "EUC_JP"); // FIXME
        $name_kana  = mb_convert_kana($name_kana, "KVC", "UTF8");
	}
}
else {
	$order_add = null;
}

// ====================================

// 失敗してから、別支払いでもう一回やってみる

if($_GET['change_payment']=='1' && $order) {
    $new_cart = Webcollect::changePayment($order);
    $_SESSION['customer'] = $new_cart['items'];
    $_SESSION['check_list'] = $new_cart['info'];
//    var_dump($_SESSION);

    if($new_cart) {
        header("location: /cart.php");
    }
    exit;
}

if($_GET['recreate_cart']=='1' && $order) {
    $new_cart = Webcollect::recreateCart($order);
    $_SESSION['customer'] = $new_cart['items'];
    $_SESSION['check_list'] = $new_cart['info'];
    //var_dump($_SESSION);

    if($new_cart) {
        header("location: /cart.php");
    }
    exit;
}

// ====================================

// webcollectから戻る時に

if($_GET['sts']) {
	$status_message = "";
    $passwd_ok = true;
    $need_form = false; // no form

	if($_GET['sts']=='s') {
        if($order['settle_result']==WEBCOLLECT_STATUS_PAID_OK) {
            $status_message = Webcollect::messageSuccess(WEBCOLLECT_MSG_STATUS_SUCCESS);
        }
        else {
            $status_message = Webcollect::messageWarning(sprintf(WEBCOLLECT_MSG_ERR_SYS_CODE, $order['settle_detail']));
            $need_form = true;
        }
	}
    else if($_GET['sts']=='f') {
        if($order['settle_result']==WEBCOLLECT_STATUS_PAID_ERR) {
            if($order['settle_detail']==11) {
                $status_message = Webcollect::messageError(WEBCOLLECT_MSG_ERR_USR);
            }
            else {
                $status_message = Webcollect::messageError(WEBCOLLECT_MSG_ERR_SYS);
            }
            $need_form = true;
        }
        else {
            $status_message = Webcollect::messageWarning(sprintf(WEBCOLLECT_MSG_ERR_SYS_CODE, $order['settle_detail']));
            $need_form = true;
        }
    }
    else if($_GET['sts']=='c') {
        $need_form = true;
    }
    else {
        // 知らないステータスの場合は
        $passwd_ok = false;
    }
}
else {
    // statusのパラメターがない場合は
    // 例えば、支払いが終わって、メールのリンクをもう一回クリックする時に

    if($order['settle_result']==WEBCOLLECT_STATUS_PAID_OK) {
        $status_message = Webcollect::messageSuccess(WEBCOLLECT_MSG_PAID);
        $passwd_ok = true;
    }
    else if($order['settle_result']==WEBCOLLECT_STATUS_PAID_ERR) {
        $need_form = true;

        if($passwd_ok) {
            if($order['settle_detail']==11) {
                $status_message = Webcollect::messageError(WEBCOLLECT_MSG_ERR_USR);
            }
            else {
                $status_message = Webcollect::messageError(WEBCOLLECT_MSG_ERR_SYS);
            }
        }
    }
    else if(Webcollect::isCancelled($order['sells_num'], $order_add['kojin_num'])) {
        $status_message = Webcollect::messageError(WEBCOLLECT_MSG_CANCELLED);
        $passwd_ok = false;
        $need_form = false;
        $show_loginform = false;
        $show_recreate_cart_btn = true;
    }
}

// ====================================

// Payment フォーム
if($need_form && $order && $order_add && $order['settle_result']!=WEBCOLLECT_STATUS_PAID_OK) {
    $url = WEBCOLLECT_URL;
    $auto_url = WEBCOLLECT_AUTO_URL;
    $trader_code = WEBCOLLECT_TRADER_CODE;
    $transac_no = Webcollect::setTransacNo($order_no, $order['sells_num'], $order_add['kojin_num'], $order['transac_no']);
	$payment_form = <<<EOT
<div class="webcollect-block go">
    <FORM NAME="UserForm" ACTION="$url" METHOD="post" accept-charset="UTF-8">
        <INPUT TYPE="hidden" NAME="TRS_MAP" VALUE="V_W02">
        <INPUT TYPE="hidden" NAME="trader_code" VALUE="$trader_code">
        <INPUT TYPE="hidden" NAME="order_no" VALUE="$transac_no">
        <INPUT TYPE="hidden" NAME="goods_name" VALUE="Ozzy's お買い上げ商品">
        <INPUT TYPE="hidden" NAME="settle_price" VALUE="{$order['all_price']}">
        <INPUT TYPE="hidden" NAME="buyer_name_kanji" VALUE="$name_kanji">
        <INPUT TYPE="hidden" NAME="buyer_tel" VALUE="{$order_add['tel1']}-{$order_add['tel2']}-{$order_add['tel3']}">
        <INPUT TYPE="hidden" NAME="buyer_email" VALUE="{$order_add['email']}">
        <INPUT TYPE="hidden" NAME="buyer_name_kana" VALUE="$name_kana">
        <INPUT TYPE="hidden" NAME="payment_method" VALUE="0">
        <INPUT TYPE="hidden" NAME="return_url"  VALUE="$auto_url">
        <INPUT TYPE="hidden" NAME="success_url" VALUE="$host/endhtml/order_card.php?sts=s&on=$order_no">
        <INPUT TYPE="hidden" NAME="failure_url" VALUE="$host/endhtml/order_card.php?sts=f&on=$order_no">
        <INPUT TYPE="hidden" NAME="cancel_url"  VALUE="$host/endhtml/order_card.php?sts=c&on=$order_no">

        <p>下記の「クレジット決済」ボタンを押し、<BR>クレジットカードでのお支払い手続きを進めて下さい。</p>
        <INPUT TYPE="submit" class="btn" VALUE="クレジット決済">
    </FORM>
</div>

<div class="webcollect-block change-payment">
    <p>別のお支払い方法の変更する場合は<br>下記の「支払い方法の変更」ボタンを押して下さい。<br>ご注文をキャンセルさせて戴き、新たにお手続きしていただきます。</p>
    <a href="?change_payment=1&on=$order_no" class="btn">支払い方法の変更</a>
</div>
EOT;
}

// エラーメッセージ表示
if(count($ERROR)>0) {
	// NOT OK
	$msg = "";
	foreach($ERROR as $err) {
		$msg .= "$err<br>";
	}
    $errors = Webcollect::messageError($msg);
}
else if(count($WARNING)>0) {
	// NOT OK
	$msg = "";
	foreach($WARNING as $err) {
		$msg .= "$err<br>";
	}
    $errors = Webcollect::messageWarning($msg);
}
else {
	// OK
	$errors = "";
}

// PASSWORD OK?
if($passwd_ok) {
    // パスワードがOkです
    $thanks_content = <<<EOT
    <TABLE class="table-resp table-resp-nogap">
        <TBODY>
            <TR bgcolor="#ffcc00">
                <TD><B>●ご注文ありがとうございました。</B></TD>
            </TR>
            <TR bgcolor="#ffffff">
                <td>
                    <div class="end-order-container">
                        <p>
                            □<FONT size="+0" color="#ff6633">ご確認下さい！</FONT>
                        </p>

                        <p>
                            <FONT color="#0099cc">確認メール</FONT>を送信させていただきました。<BR>
                            <FONT color="#ff6633">メールが届かない場合は、</FONT>
                        </p>
                        <ul>
                            <li>・入力したメールアドレスが間違っている</li>
                            <li>・携帯のメールアドレスでドメイン指定されている</li>
                            <li>
                                可能性がありますので、<FONT color="#ff6633">お手数ですが、ご連絡下さい。</FONT><BR>
                                  また、内容に間違いなどがあった場合もご連絡下さい。
                            </li>
                        </ul>

                        <p>
                            <B>今後の取引の流れ</B>
                        </p>
                        <p>
                            <IMG src="../riyou/image/flow-creditcard.gif" class="img-responsive" alt="取引の流れ">
                        </p>
                    </div>

                    $payment_form

                    $change_payment_form
                </td>
            </TR>
        </TBODY>
    </TABLE>
EOT;
}
else if($show_loginform) {
    // パスワードの確認が必要です
    $thanks_content = <<<EOT
    <TABLE class="table-resp table-resp-nogap">
        <TBODY>
            <TR bgcolor="#ffcc00">
                <TD><B>ご注文ありがとうございました。</B></TD>
            </TR>
            <TR bgcolor="#ffffff">
                <td>
                    <div class="end-order-container">
                        <p class="text-center">本人確認のため、メールで送られたパスワードを以下に入力してください。</p>
                        <form action="?on=$order_no" method="POST" class="webcollect-passwd">
                            <input type="text" class="input-block" name="passwd" value="" required />
                            <br>
                            <center><button type="submit" class="btn">確認</button></center>
                        </form>
                    </div>
                </td>
            </TR>
        </TBODY>
    </TABLE>
EOT;
}
elseif($show_recreate_cart_btn) {
    $thanks_content = <<<EOT
        <div class="webcollect-block change-payment">
            キャンセルされました商品を再度ご購入する場合は、以下の「再購入する」ボタンを
    クリックしてください。
            <br><br>
            <a href="?on={$order_no}&recreate_cart=1" class="btn">再購入する</a>
        </div>
EOT;
}

$headimg = read_file_make_html("headimg.php");
$headmsg = read_file_make_html("headmsg.php");
//$loginmsg_resp = read_file_make_html("loginmsg_resp.php");
$menulist_resp = read_file_make_html("menulist_resp.php");
$osusume = read_file_make_html("osusume_resp.php");


// =============================================================================
//                                 MAIN PAGE
// =============================================================================


// コンテンツのHTML
echo <<<EOT
<!DOCTYPE html>
<HTML>
<HEAD>
	<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<meta name="viewport" content="initial-scale=1.0,width=device-width" />
	<META http-equiv="Content-Style-Type" content="text/css">
	<TITLE>ご注文有り難うございました オジーズ</TITLE>
	<SCRIPT type="text/javascript" src="../sub/js_s.js" charset="UTF-8"></SCRIPT>
	<SCRIPT type="text/javascript" src="../sub/swf.js" charset="UTF-8"></SCRIPT>
	<LINK rel="stylesheet" href="../sub/style.css" type="text/css">

	<!-- new CSS -->
	<link rel="stylesheet" href="../sub/responsive.css">
	<script src="../sub/jquery-3.1.0.min.js"></script>
	<script src="../sub/ozzys.js"></script>
</HEAD>
<BODY>
	<div class="page">
		<header>
			<div id="navigation-mobile"></div>

			<div class="stripes"></div>
			<div class="clearfix banner" style='background:black url(../images/{$headimg}) top right no-repeat;'>
				<div class="logo">
					<a href="/"><img src="../images/ozzys-logo.gif" class="img-responsive" alt="ロゴ" /></a>
				</div>
				<div class="blockquote">
					<blockquote class="headmsg">{$headmsg}</blockquote>
				</div>
			</div>

			<div id="navigation-pc">
				<nav id="navigation-header" class="clearfix" style="display: none">

					<div id="mobile-menu-overlay"></div>

					<div id="mobile-menu-left" class="mobile-menu">
						<div id="mobile-button-left" class="mobile-button" data-menu="left"></div>
						<div class="mobile-content">
							<div id="mobile-navbox"></div>
						</div>
					</div>

					<div id="mobile-menu-right" class="mobile-menu">
						<div id="mobile-button-right" class="mobile-button" data-menu="right">サイドを見る</div>
						<div class="mobile-content">
							<div id="mobile-col-right"></div>
						</div>
					</div>

                    <div id="loginMsg"></div>
					<script>
                        $.ajaxSetup({
                            'beforeSend' : function(xhr) {
                                xhr.overrideMimeType('text/html; charset=UTF-8');
                            },
                        });
                        $("#loginMsg").load('../loginmsg_resp.php');
                    </script>
				</nav>
			</div>
		</header>

		<div id="mobile-login-menu"></div>

		<div id="main">

			<!--======================================================================-->
			<!--left-->
			<!--======================================================================-->
			<div id="pc-col-left" class="col col-left stripes">
				<nav id="navbox">
					{$menulist_resp}
				</nav> <!-- #navbox end -->
			</div> <!-- end .col-left -->


			<!--======================================================================-->
			<!--middle-->
			<!--======================================================================-->
			<div class="col col-middle">

				<h2 class="sub-title-prod">ご注文</h2>

				$status_message

				$errors

				$thanks_content

			</div> <!-- end .col-middle -->

			<!--======================================================================-->
			<!--right-->
			<!--======================================================================-->
			<div id="pc-col-right" class="col col-right">
				<div id="col-right">
                    {$osusume}
				</div> <!-- end #col-right -->
			</div> <!-- end .col-right -->

		</div> <!-- end #main -->

		<footer>
		</footer>
	</div> <!-- end .page -->

</BODY>
</HTML>
EOT;












/**
 * ファイルを読み込んで変数に入れる
 * SSIで表示していたファイルを表示する為に使用
 * @param stringg $file_path_name
 * @return string $html
 */
function read_file_make_html($file_path_name) {
    //var_dump(ini_get('allow_url_fopen'));

    if($_SERVER['SERVER_NAME'] == 'ozzys.jp') {
        $server_url = 'https://ozzys.jp';
    }
    else {
        $server_url = 'http://'.$_SERVER['SERVER_NAME'];
    }
    $file_url = $server_url.'/'.$file_path_name.'?session_id='. session_id();
//    $content = $file_url;

    $content = file_get_contents($file_url);

    return $content;
}
