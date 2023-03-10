<?php

// ****************
// シモンの備考：
// クロネコWEBコレクトの実装の為にこのファイルの仕組みを save() と send() に分ける
// save() ファンクションはDBにデータを保存し、メール内容を準備する
// send() ファンクションは save() 時に準備された メール内容を利用し 送る
// ****************


/**
 * 注文をDBに保存する
 *
 * @param String $idpass
 * @return integer|void (エラー場合は、void)
 */
function order_save($idpass) {
	global $PHP_SELF,$db,$PRF_L,$T_TIME_L,$R_POINT_L,$UN_L,$T_SOU_L,$PAY_L,$tui_sou,$daibiki,$h_tax,
	$tax,$point_b,$point_c,$admin_mail,$admin_name,$m_footer;

	//	送料無料設定定数変数設定
	//	add ookawara 2016/07/25
	$souryoufree = 0;
	if (defined("SOURYOUFREE")) {
		$souryoufree = SOURYOUFREE;
	}
	$souryoufreeprice = 0;
	if (defined("SOURYOUFREEPRICE")) {
		$souryoufreeprice = SOURYOUFREEPRICE;
	}

	if (!$_SESSION['customer'] || $_SESSION['customer_check'] != 1) {
		header ("Location: $PHP_SELF\n\n");
		exit;
	} else {
		$customer = $_SESSION['customer'];
	}

	$lockdir = "./lock/o";
	$lock_check = lockfile($lockdir);
	if ($lock_check == 0) {
		header ("Location: $PHP_SELF?mode=check\n\n");
		exit;
	}

	//	トランザクション開始
	pg_query("BEGIN");

	//	商品番号決め　（時間）
	$sells_num = date("ymdHis");

	if ($_SESSION['check_list']) {
		$check_list = $_SESSION['check_list'];
		list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$email,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$point,$t_time,$r_point,$g_point,$msg,$pay_type,$delivery) = explode("<>",$check_list);
	}

	//	非会員処理
	if (!$idpass) {
		$sql = "SELECT MAX(kojin_num) AS max FROM kojin WHERE kojin_num>='600001';";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['max'] + 1;
		}
		if (!$kojin_num || $kojin_num < 600000) { $kojin_num = 600001; }
		$sql =  "INSERT INTO kojin VALUES (" .
				"'$kojin_num','$name_s','$name_n','$kana_s','$kana_n','0'," .
				"'1000-01-01','$zip1','$zip2','$prf','$city','$add1'," .
				"'$add2','$tel1','$tel2','$tel3','$fax1','$fax2'," .
				"'$fax3','$email','$email','1000-01-01','2','0'," .
				"'0');";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "お客様情報を登録できませんでした。"; }
	}

	//	住所登録
	if (!$ERROR) {
		//	add_num取り出し
		$sql = "SELECT MAX(add_num) AS max FROM add;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$add_num = $list['max'];
		}
		$add_num += 1;

		//	保存
		if (!$g_point) { $g_point = "0"; }
		if (!$delivery) { $delivery = "0"; }
		if ($delivery == 1) { $t_time = "0"; }
		$sou = $UN_L[$prf];
		if (!$sou) { $sou = 0; }

		$sql =  "INSERT INTO add VALUES (" .
				"'$add_num','$kojin_num','$name_s','$name_n','$kana_s','$kana_n'," .
				"'$zip1','$zip2','$prf','$city','$add1','$add2'," .
				"'$tel1','$tel2','$tel3','$fax1','$fax2','$fax3'," .
				"'$email','$t_time','$msg','$g_point','$pay_type','$delivery','$sou');";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "送り先情報を記録できませんでした。"; }
	}

	//	購入商品登録
	if (!$ERROR) {
		$all_price = 0;
		$goods = "";
		$un = 0;
		$CART = $customer;
		$cart_items = array();
		foreach ($CART AS $list_num => $buy_num) {
			if (!$list_num || !$buy_num) { continue; }
			//	商品単価、番号、商品数の取り出し
			$sql  = "SELECT a.price, a.goods_num, a.stock, d.goods_name, d.size, d.color, a.class_m, a.pluid, d.maker_num, a.set_flag" .
					", d.free_postage".					//	add ookawara 2016/07/25
					" FROM goods a, list d" .
					" WHERE a.pluid=d.pluid AND d.list_num='$list_num' LIMIT 1;";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);
				$price = $list['price'];
				$goods_num = $list['goods_num'];
				$stock = $list['stock'];
				$goods_name = $list['goods_name'];
				$size = $list['size'];
				$color = $list['color'];
				$class_m = $list['class_m'];
				$pluid = $list['pluid'];
				$maker_num = $list['maker_num'];
				$set_flag = $list['set_flag'];
				if (!$pluid) {
					$ERROR[] = "商品情報が読み込めませんでした。";
				}
				else {
					$cart_items[] = $list; // add simon 2018-07-06
				}

				$free_postage = $list['free_postage'];	//	add ookawara 2016/07/25
			}
			//	メーカー名の読み込み
			$sql = "SELECT maker_name FROM maker WHERE maker_num='$maker_num'";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);
				$maker_name = $list['maker_name'];
			}
			//	購入商品の登録
			if (!$price) { $price = "0"; }
			if (!$buy_num) { $buy_num = "0"; }
			if (!$send) { $send = "0"; }
			$sql =  "INSERT INTO sells VALUES (" .
					"'$sells_num','$kojin_num','$add_num','$list_num','$price'," .
					"'$buy_num','$send','1000-01-01','0','$tax');";
			if (!$result = pg_query($db,$sql)) { $ERROR[] = "購入商品が記録できませんでした。"; }

			//	メール書き込み部分
			if ($h_tax == 1) {
				$price_h = $price + floor(($price * $tax) + 0.5);
			} else {
				$price_h = $price;
			}

			//	add ookawara 2016/07/29
			$check_price = $price_h;

			$syoukei_h = $price_h * $buy_num;
			$syoukei_h = number_format($syoukei_h) . "円";

			$syoukei = $price_h * $buy_num;
			$all_price += $syoukei;

			if ($size) { $sizes = "\nサイズ：$size"; } else { $sizes = ""; }
			if ($color) { $colors = "\nカラー：$color"; } else { $colors = ""; }


			if ($goods_a) {
				$goods_a .= "\n";
				$goods_b .= "\n";
			}

			$goods_a .= <<<OZZYS
ポス番号：$pluid
商品番号：$list_num
メーカー名：$maker_name
商品名：$goods_name $sizes $colors
数量：$buy_num
金額：$syoukei_h
--------------------------------------------------------
OZZYS;

			$goods_b .= <<<OZZYS
商品番号：$list_num
商品名：$goods_name $sizes $colors
数量：$buy_num
金額：$syoukei_h
--------------------------------------------------------
OZZYS;

			//	在庫減
			$stock = $stock - $buy_num;
			$sql =  "UPDATE goods SET " .
					" stock='$stock'" .
					" WHERE goods_num='$goods_num';";
			if (!$result = pg_query($db,$sql)) { $ERROR[] = "在庫を確認すことができませんでした。"; }

			//	売りリスト
			$sql = "SELECT buy_num FROM total WHERE list_num='$list_num' AND s_date='now()';";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);
				$buy_num_ = $list['buy_num'];
			}
			if ($buy_num_ <= 0) {
				//	新規
				$sql =  "INSERT INTO total VALUES (" .
						"'$list_num','$buy_num','now()');";
				if (!$result = pg_query($db,$sql)) { $ERROR[] = "お買いあげ商品を登録することができませんでした。"; }
			} else {
				//	追加
				$buy_num_ += $buy_num;
				$sql =  "UPDATE total SET " .
						" buy_num='$buy_num_'" .
						" WHERE list_num='$list_num' AND s_date='now()';";
				if (!$result = pg_query($db,$sql)) { $ERROR[] = "お買いあげ商品を追加登録することができませんでした。"; }
			}
		}
	}

	//	ポイント減
	if ($g_point) {
		//	ポイント確認
		$sql = "SELECT point FROM kojin WHERE kojin_num='$kojin_num' AND saku!='1' LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$point = $list['point'];
		} else {
			$ERROR[] = "ポイント確認できませんでした。";
		}
		$point = $point - $g_point;
		if ($point <= 0) { $point = 0; }

		$sql =  "UPDATE kojin SET " .
				" point='$point'" .
				" WHERE kojin_num='$kojin_num' AND saku!='1';";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "ポイント変更する事ができませんでした。"; }
	}

	//	メール作業
##	$msg = eregi_replace("\r","",$msg);
	$msg = preg_replace("/\r/i","",$msg);
##	$msg = eregi_replace("\n","\n　",$msg);
	$msg = preg_replace("/\n/i","\n　",$msg);

	$syouhizei = 0;
	if ($h_tax != 1) {
		$syouhizei = $all_price * $tax;
		$syouhizei = ceil($syouhizei);
	}

	//	支払い方法＆配送方法
	if ($pay_type != 0 && $pay_type != 3) {
		unset($daibiki);
		unset($daibiki_msg);

		if ($delivery == 1) {
            // メール便の発送の場合　今後やらないので使わない！2019/03/18電話で確認済み
            // シモン：カートのフォームにdelivery項目がありませんので、1にならないです。
            // >>>
			$delivery_msg = "　　　　　メール便、利用可能なら利用する。\n";
			$title_delivery_msg = "【メール便】";
			if ($pay_type == 1) {
				$pay_msg = <<<WAKABA

送料は、メール便で発送出来るか確定しておりませんので、在庫の有無を確認後、
1営業日中に確定金額（送料を合わせたお支払い合計金額）、
お振込先をご連絡させて頂きます。しばらくお待ちください。

WAKABA;
			} elseif ($pay_type == 2) {
				$pay_msg = <<<WAKABA

送料は、メール便で発送出来るか確定しておりませんので、在庫の有無を確認後、
1営業日中に確定金額（送料を合わせたお支払い合計金額）、
現金書留の送り先をご連絡させて頂きます。しばらくお待ちください。

WAKABA;
			}
            // <<<
		} else {
			if ($pay_type == 1) {
				$pay_msg = <<<WAKABA

在庫の有無を確認後、1営業日中に確定金額（送料を合わせたお支払い合計金額）、
お振込先をご連絡させて頂きます。しばらくお待ちください。

WAKABA;
			} elseif ($pay_type == 2) {
				$pay_msg = <<<WAKABA

在庫の有無を確認後、1営業日中に確定金額（送料を合わせたお支払い合計金額）、
現金書留の送り先をご連絡させて頂きます。しばらくお待ちください。

WAKABA;
			}
		}
	} else {
		unset($pay_msg);
		$daibiki	= number_format($daibiki) . "円";
		$daibiki_msg = <<<WAKABA
代引き手数料
$daibiki
--------------------------------------------------------

WAKABA;
	}

	$souryou_area = $UN_L[$prf];
	$souryou = Souryou::calculate($cart_items, $souryou_area, $all_price);

	if ($pay_type != 0 && $pay_type != 3 && $delivery == 1) { unset($souryou); }

	//	送料の情報更新
	//	add ookawara 2016/07/26
	if (!$ERROR) {
		$sql =  "UPDATE add SET " .
				" souryou='".$souryou."'" .
				" WHERE add_num='".$add_num."'".
				" AND kojin_num='".$kojin_num."';";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "送料情報を更新する事ができませんでした。"; }
	}

	//if ($un == 1) { $unchin += $tui_sou; }				//	del ookawara 2016/09/13
	//if ($un == 1 && $unchin > 0) { $unchin += $tui_sou; }	//	add ookawara 2016/09/13		//	del ookawara 2016/10/18
	//if ($pay_type > 0 && $pay_type < 3 && $delivery == 1) { unset($souryou); }			//	del ookawara 2016/10/18
	$p_price = -($g_point/$point_c) * $point_b;
	$all_price_number = $all_price + $syouhizei + $souryou + $daibiki + $p_price;
	if ($all_price_number <= 0) { $all_price_number = 0; }

	$syouhizei	= number_format($syouhizei) . "円";
	$souryou	= number_format($souryou) . "円";
	$p_price	= number_format($p_price) . "円";
	$all_price	= number_format($all_price_number) . "円";

	// web collect >>>
	if ($pay_type == 4) {

		$new_order_no = Webcollect::makeOrderNo($sells_num, $add_num, $all_price_number);
        $new_order_passwd = Webcollect::generatePasswd();
		$order_save = Webcollect::saveOrder($sells_num, $add_num, $all_price_number, $new_order_no, $new_order_passwd);
		if (!$order_save) { $ERROR[] = "ウエブコレクト用のトークン作成に失敗しました。"; }
		else {
            // メールメッセージに注文終了画面のリンクを追加
            $pay_msg .= <<<EOT

クレジットカード決済ご利用有り難う御座います。
決済のお手続きが済んでいない場合以下のアドレスから
お手続きお願い致します。
クレジット決済完了されても、発送しない限り請求される事はございません。
https://ozzys.jp/endhtml/order_card.php?on=$new_order_no
上記のURLをアクセスするため、以下のパスワードをご利用下さい。

【{$new_order_passwd}】

EOT;
			$_SESSION['webcollect_order_no'] = $new_order_no;
		}
	}
	// <<<

	/* simon 2018-07-06 : deliveryは使いませんので、この条件が当たらない */
	if ($pay_type != 0 && $pay_type != 3 && $delivery == 1) {
		$souryou = "未確定";
		$all_price	.= " + 送料";
	}

	if ($g_point) {
		$point_m = <<<OZZYS
ポイント利用
$g_point ポイント $p_price
--------------------------------------------------------

OZZYS;
	}

	$tax_num = $tax * 100;

	$tax_msg = "";
	$tax_com = "";
	if ($h_tax != 1) {
		$tax_msg = <<<WAKABA

消費税（$tax_num%）
$syouhizei 円
--------------------------------------------------------
WAKABA;
	} else {
/*
		//	del ookawara 時期未定
		$tax_com = <<<WAKABA
レジシステムの関係上、「単品（税込み）価格の合計」と、
「お支払い合計金額」との間に誤差が生じてしまいます。
「お支払い合計金額」は、「単品（税込み）価格の合計」よりも
高くなることはございませんのでシステムの対応ができるまで、
ご了承願います。

WAKABA;
*/
	}

	//	お届け時間
	if ($delivery != 1) {
		$time_msg = <<<EOT

お届け時間
　$T_TIME_L[$t_time]
EOT;
	}

    // add simon 2019-03-18
    $delivery_company = Souryou::getDeliveryCompany($cart_items);

	//	確認メール送信
	$subject = "ご注文ありがとうございます。 - ozzys -";

	$msr = <<<OZZYS
$subject

$name_s 様 ご購入内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがご連絡お願いします。
########################################################

ご注文内容 (ご注文番号：$sells_num)

--------------------------------------------------------
$goods_b$tax_msg
送料
$souryou
--------------------------------------------------------
$daibiki_msg$point_m
合計金額
$all_price
--------------------------------------------------------
●支払い方法：{$PAY_L[$pay_type]}
●配送方法：{$delivery_company}{$delivery_msg}

レジシステムの関係上、「単品（税込み）価格の合計」と、
「お支払い合計金額」との間に誤差が生じてしまいます。
「お支払い合計金額」は、「単品（税込み）価格の合計」よりも
高くなることはございませんのでシステムの対応ができるまで、
ご了承願います。
$pay_msg
########################################################

送り先情報

--------------------------------------------------------

氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email
$time_msg
------------------------------------------------------
ご意見ご要望
　$msg

########################################################

●水曜日のお荷物の発送はできません（定休日のため）！！！

火曜日のPM3:00以降にいただだきましたご注文の発送は木曜日以降になります。
ご注文は24時間、365日承ります。
また、出荷の際にお送りする発送メールも木曜日以降になります。
ご不便をお掛けしますが、よろしくお願いいたします。

$m_footer

OZZYS;

/*

◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

12月31日（月）、1月1日（火）、1月7日（月）、1月8日（火）は
誠に勝手ながらお休みさせていただきます。
また、新年1月2日～1月3日は新春初売りセールの為に
通信販売の出荷業務もお休みさせていただきます。

12月30日（日）PM5：00～1月3日（木）の間に頂いた
ご注文の商品の出荷と発送メールは1月4日（金）以降になります。
また、1月6日（日）PM5:00～1月9日（水）の間に頂いた
ご注文の商品の出荷と発送メールは1月10日（木）以降になります。
ご迷惑をお掛けいたしますが予めご了承下さい。

◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇◆◇

*/

	// **************************************************
	// MAIL TO CLIENT
	// **************************************************
	if (!$ERROR) {
		$send_email = $admin_mail;
		$send_name = $admin_name;
		// save in session!
		$_SESSION['order_email_to_client'] = OrderMail::make_params($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
	}

	//	受注メール送信
	$subject = "注文番号[ $sells_num ] $name_s" . "様(No." . $kojin_num. ")からのご注文です。".$title_delivery_msg;

	$addr = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($addr);
	if (!$host) { $host = $addr; }

	$msr = <<<OZZYS
$subject
########################################################

ご注文内容

--------------------------------------------------------
$goods_a$tax_msg
送料
$souryou
--------------------------------------------------------
$daibiki_msg$point_m
合計金額
$all_price
--------------------------------------------------------
●支払い方法：{$PAY_L[$pay_type]}
●配送方法：{$delivery_company}{$delivery_msg}

########################################################

送り先情報

--------------------------------------------------------

氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email
$time_msg
------------------------------------------------------
ご意見ご要望
　$msg

########################################################
$host ($addr)

OZZYS;

	if (!$ERROR) {
		$send_email = $email;
		$send_name = "$name_s $name_n";
		$email = $admin_mail;
		// save in session!
		$_SESSION['order_email_to_admin'] = OrderMail::make_params($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
	}
	unlockfile($lockdir);

	if ($ERROR) {
		//	ロールバック
		pg_query("ROLLBACK");

		// session mail data clear
		unset($_SESSION['order_email_to_client']);
		unset($_SESSION['order_email_to_admin']);
		unset($_SESSION['webcollect_order_no']);

		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF\n\n");
		exit;

	} else {
		//	トランザクション終了
		pg_query("COMMIT");

		//	セッション解除
		unset($_SESSION['customer']);
		unset($_SESSION['customer_check']);
		unset($_SESSION['b_url']);
		unset($_SESSION['id']);
		unset($_SESSION['check_list']);
		unset($_SESSION['ERROR']);
	}

	// for redirection from main program (cart function)
	return ($pay_type * 1); // force cast into integer
}


//	ロック
function lockfile($lockdir) {

	//	デッドロックチェック
	if (file_exists($lockdir)) {
		$now = time();
		$list = stat($lockdir);
		$mtime = $list[mtime];
		$sa = $now - $mtime;
		if ($sa > 10) {
			rmdir($lockdir);
		}
	}

	//	ロック設定
	$lock_check = 0;
	for($i=0; $i<5; $i++) {
		if (!file_exists($lockdir)) {
			mkdir($lockdir,0777);
			chmod($lockdir,0777);
			$lock_check = 1;
			break;
		}
		else {
			sleep(1);
		}
	}

	return $lock_check;

}


//	アンロック
function unlockfile($lockdir) {

	if (file_exists($lockdir)) {
		rmdir($lockdir);
	}

}


/**
 * 注文が終わるときに、メールを送ります
 *
 * @param int $pay_type_
 * @return void
 */
function order_send($pay_type_) {
	global $PHP_SELF;

	// first send to client
	list($check, $ERROR) = OrderMail::sendToClient( $_SESSION['order_email_to_client'] );
	if($check) {
		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	// then send to Admin
	list($check, $ERROR) = OrderMail::sendToAdmin( $_SESSION['order_email_to_admin'] );
	if($check) {
		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($pay_type_ === 0 || $pay_type_ === 3) {
		header ("Location: /endhtml/order2.htm\n\n");
	} elseif ($pay_type_ === 4) {
		//	クロネコwebコレクト用追加
		header ("Location: /endhtml/order_card.php\n\n");
	} else {
		header ("Location: /endhtml/order1.htm\n\n");
	}
}
