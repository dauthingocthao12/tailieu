<?php
/*
 * Webcollect用のライブラリー
 */

define('WEBCOLLECT_TRADER_CODE', 230596503);
define('WEBCOLLECT_STATUS_UNPAID',   0);
define('WEBCOLLECT_STATUS_PAID_OK',  1);
define('WEBCOLLECT_STATUS_PAID_ERR', 2);

define('WEBCOLLECT_MSG_WEIRD', 'お手数ですが、ご注文確認メールに記載させて戴いている、支払いリンクから再度お手続きをしてください。');
define('WEBCOLLECT_MSG_PAID', 'ご注文のお支払い手続きが完了しております。<br>引き続き<b>フィッシングプロショップ オジーズ</b>を宜しくお願い致します。');
define('WEBCOLLECT_MSG_ERR_USR', 'お客様がご入力して頂いた情報に誤りがあるかご利用ができません。<br>お手数ですが、「クレジット決済」ボタンから再度お手続きをして頂くか、<br>「支払い方法の変更」ボタンを押して、別のお支払い方法でお手続きをしてください。');
define('WEBCOLLECT_MSG_ERR_SYS', '大変申し訳ございませんが、システムの問題があり、手続きが完了できませんでした。<br>お手数ですが、「クレジット決済」ボタンから再度お手続きをして頂くか、<br>「支払い方法の変更」ボタンを押して、別のお支払い方法でお手続きをしてください。');
define('WEBCOLLECT_MSG_STATUS_SUCCESS', 'お支払いありがとうございました。<br>引き続きフィッシングプロショップ オジーズを宜しくお願い致します。');
define('WEBCOLLECT_MSG_ERR_SYS_CODE', 'システムエラー (%s)<br>お手数ですが、ご注文確認メールに記載させて戴いている、支払いリンクから再度お手続きをしてください。');
define('WEBCOLLECT_MSG_CANCELLED', 'ご注文がキャンセルされている為、お支払い手続きを進めることができません。<br>お手数ですが再度ご注文確認メールを確認して頂くか、ご注文をし直してください。');
define('WEBCOLLECT_MSG_PASSWD', 'パスワードが間違っております。');
define('WEBCOLLECT_MSG_DATANOTFOUND', 'お客様情報が確認できませんでした。');

class Webcollect {

	/**
	 * 開発環境のURLか？
     * 本番か？
	 *
	 * @global string $URL
	 * @return string
	 */
	static function getHost()
	{
		global $URL;

		if($_SERVER['HTTP_HOST']=="ozzys.jp") {
            return $URL;
        }
		else {
            return "http://".$_SERVER['HTTP_HOST'];
        }
	}


    static function generatePasswd()
    {
        return rand(1000, 9999);
    }


    static function checkPasswd($order_no_, $passwd_)
    {
        global $db;

        $sql = "SELECT * FROM webcollect WHERE order_no = '$order_no_' AND passwd='$passwd_' LIMIT 1";
        $rs = pg_query($db, $sql);

        return ($rs && pg_num_rows($rs)==1);
    }


	/**
	 * 新しいクレジットカード支払いのために、接続番号を作るメソッド
	 *
	 * @param int $sells_num_ (date format: YmdHis)
	 * @param int? $kojin_num_
	 * @param int $all_price_
	 * @return string
	 */
	static function makeOrderNo($sells_num_, $kojin_num_, $all_price_)
	{
		$hash = sha1($sells_num_.$kojin_num_.$all_price_);
		return strtoupper(substr($hash, 0, 23));
	}


    /**
	 * webcollectのトランサクション番号を作成
     * テーブルも更新される
	 *
     * @param string webcollectテーブルのプライマリーキー
	 * @param int $sells_num_ (date format: YmdHis)
	 * @param int $kojin_num_
	 * @param string $transac_no_ null も可能
     *   形式：AAAAAAAAAAAABBBBBBCCC
     *     AAAAAAAAAAAA は sells_num
     *     BBBBBB は kojin_num
     *     CCC は counter
	 * @return string or null if error
	 */
	static function setTransacNo($order_no_, $sells_num_, $kojin_num_, $transac_no_)
	{
        global $db;

        $counter = 0;
        if($transac_no_ !== null) {
            $counter = intval( substr($transac_no_, 18, 3) );
        }
        $new_counter = $counter + 1;
        $transac_no = $sells_num_ . $kojin_num_ . sprintf("%03d", $new_counter);

        // TODO add checksum if possible

        $sql = "UPDATE webcollect SET transac_no = '$transac_no' WHERE order_no = '$order_no_'";
        $ok = pg_query($db, $sql);

		return $ok!==false ? $transac_no : null;
	}


	/**
	 * 新しい注文のトランサクションを登録する
	 *
	 * @global db $db
	 * @param int $sells_num_
	 * @param int $add_num_
	 * @param int $all_price_
	 * @param string $new_order_no_ (23文字)
	 * @param string $new_order_passwd_ (4数字)
	 * @return boolean
	 */
	static function saveOrder($sells_num_, $add_num_, $all_price_, $order_no_, $new_order_passwd_)
	{
		global $db;

		$new_transac_sql = "INSERT INTO webcollect "
				. "(sells_num, add_num, all_price, order_no, passwd) "
				. "VALUES "
				. "($sells_num_, $add_num_, $all_price_, '$order_no_', '$new_order_passwd_')";
//		die($new_transac_sql);

		if (!$result = pg_query($db, $new_transac_sql)) {
            return false;
        }
		else {
            return true;
        }
	}


	/**
	 * DBから注文データを読み込む
	 * @global db $db
	 * @param string $order_no_ (23文字)
	 * @return array (or null if error)
	 */
	static function readOrder($order_no_)
	{
		global $db;

		$sql = "SELECT * FROM webcollect WHERE order_no = '$order_no_' LIMIT 1";
		$rs = pg_query($db, $sql);
		if(!$rs) {
            return null;
        }

		return pg_fetch_assoc($rs);
	}


	/**
	 * DBから注文の住所を読み込む
	 * @global db $db
	 * @param int $add_num_
	 * @return array (or null if error)
	 */
	static function readAdd($add_num_)
	{
		global $db;

		$sql = "SELECT * FROM \"add\" WHERE add_num = $add_num_ LIMIT 1";
//		die($sql);
		$rs = pg_query($db, $sql);
		if(!$rs) {
            return null;
        }

		return pg_fetch_assoc($rs);
	}


    /**
	 * DBから注文の内容を読み込む
	 * @global db $db
	 * @param int $sells_num_
	 * @param int $kojin_num_
	 * @return array (or null if error)
	 */
	static function readSells($sells_num_, $kojin_num_)
	{
		global $db;

		$sql = "SELECT * FROM \"sells\" WHERE sells_num = '$sells_num_' AND kojin_num = $kojin_num_";
//		die($sql);
		$rs = pg_query($db, $sql);
		if(!$rs) {
            return null;
        }

        $data = array();
        while($row = pg_fetch_assoc($rs)) {
            $data[] = $row;
        }

		return $data;
	}


    /**
     * YmdHis の日付けを日本語に変換
     * @param string $order_settle_date_ (クロネコからsettle_dateデータ)
     * @return string
     */
    static function formatSettleDate($order_settle_date_) {
        return preg_replace("/^(\d{4})(\d{2})(\d{2})(\d{2})(\d{2}).*/", "$1年 $2月 $3日 $4時 $5分", $order_settle_date_);
    }


    /**
     * 管理画面(一覧)にカード決済のステータス表示
     * @param int $result_ settle_result
     * @return string
     */
    static function masterPaymentIndicator($result_)
    {
        if($result_==WEBCOLLECT_STATUS_PAID_OK) {
            $payments = "<font color='green' title='決済終了'>カード払い</font>";
        }
        elseif($result_==WEBCOLLECT_STATUS_PAID_ERR) {
            $payments = "<font color='red' title='エラー'>カード払い</font>";
        }
        elseif($result_==WEBCOLLECT_STATUS_UNPAID) {
            $payments = "<font color='orange' title='未決済'>カード払い</font>";
        }

        return $payments;
    }


    /**
     * 管理画面(一覧)にカード決済のステータス表示
     * @param int $result_ settle_result
     * @param int $detail_ settle_detail
     * @return string
     */
    static function masterPaymentIndicatorDetails($result_, $detail_)
    {
        if($result_==WEBCOLLECT_STATUS_PAID_OK) {
            $payments = "<font color='green'>決済終了</font>";
        }
        elseif($result_==WEBCOLLECT_STATUS_PAID_ERR) {
            $detail = "";
            if($detail_) {
                $detail_text = self::settleDetailText($detail_);
                $detail = "未決済 ($detail_: $detail_text)";
            }
            $payments = "<font color='red'>$detail</font>";
        }
        elseif($result_==WEBCOLLECT_STATUS_UNPAID) {
            $payments = "<font color='orange'>未決済</font>";
        }

        return $payments;
    }


    /**
     * 別支払い方法にするために、前の注文をキャンセル
     *
     * @global db $db
     * @param int $sells_num_
     * @param int $kojin_num_
     * @return boolean
     */
    static function cancelSells($sells_num_, $kojin_num_)
    {
        global $db;
        $success = false;

        // 参考： master/goods_2.inc @ l.809~829
        $sql = "select MAX(point_num) from points;";
        $res1 = pg_query($db, $sql);
        $count = pg_numrows($res1);

        if ($count >= 1) {
            list($_point_num) = pg_fetch_array($res1, 0);
            $point_num = $_point_num + 1;
        }
        else {
            $point_num = 1;
        }

        $sql = "insert into points values (" .
                " '$point_num'," .
                " '$kojin_num_'," .
                " '$sells_num_'," .
                " '0');";
        $res2 = pg_query($db, $sql);

        if ($res2 !== false) {
            // sendは商品をキャンセルする
            // stateは注文完了にする
            $sql = "UPDATE sells SET send='2', state='1' WHERE sells_num='$sells_num_' AND kojin_num=$kojin_num_";
            $rs = pg_query($db, $sql);
            if($rs !== false) {
                $success = true;
            }
        }

        return $success;
    }


    /*
     * 注文がキャンセルされているか確認
     * pointテーブルに同じsells_numで判断する
     * @param $sells_num_ int
     * @param $kojin_num_
     * @return boolean
     */
    static function isCancelled($sells_num_, $kojin_num_) {
        global $db;
//        print $sells_num_;
//        print $kojin_num_;

        $sql = "SELECT * FROM points WHERE kojin_num=$kojin_num_ AND sells_num='$sells_num_' ";
//        print $sql;
        $res = pg_query($db, $sql);

        return pg_numrows($res)>0;
    }


	/**
	 * Webcollectから自動返事対応
     * 注文のステータスを変更する
	 *
	 * @global db $db
	 * @param string $transac_no_
	 * @param array$post_
	 * @return boolean
	 */
	static function transacAuto($transac_no_, $post_)
	{
		global $db;

		$sql = "UPDATE webcollect SET "
				. "settle_price={$post_['settle_price']}, "
				. "settle_date={$post_['settle_date']}, "
				. "settle_result={$post_['settle_result']}, "
				. "settle_detail={$post_['settle_detail']}, "
				. "settle_method={$post_['settle_method']}, "
				. "upd_date = NOW() "
				. "WHERE transac_no='$transac_no_'";

		$rs = pg_query($db, $sql);

		return $rs?true:false;
	}


    /**
     * 注文の支払いが失敗した後に
     * もう一回別支払い方法を使うために
     *
     * @global db $db
     * @param array $order_ (webcollect の一行)
     * @return array (or false if error) 詳細はrecreateCartメソッドに参考
     */
    static function changePayment($order_)
    {
        global $admin_mail, $admin_name, $m_footer;

        $add_data = self::readAdd($order_['add_num']);
        $client_name = $add_data['name_s'].' '.$add_data['name_n'];
        $cancel_status = self::cancelSells($order_['sells_num'], $add_data['kojin_num']);

        if(!$cancel_status) {
            return false;
        }

        // send order cancel message
        $subject = "ご注文がキャンセルされました。 - ozzys -";
        $msr = <<<EOT
$client_name 様

お支払い情報を変更した為、以下のご注文番号のご注文をキャンセルとさせて頂きました。

########################################################

ご注文番号：{$order_['sells_num']}

########################################################

※このメールは自動返信メールですのでご返信の必要は御座いません。

$m_footer
EOT;

        // mail to customer
        $send_email = $admin_mail;
		$send_name = $admin_name;
        $email = $add_data['email'];
        $send1 = send_email($send_email, $send_name, 0, '', $email, $subject, $msr);

        // mail to admin
        $send_email = $add_data['email'];
        $send_name = $client_name;
        $email = $admin_mail;
        $send2 = send_email($send_email, $send_name, 0, '', $email, $subject, $msr);

        if($send1 || $send2) {
            // send error?
            return false;
        }

        $sells_data = self::readSells($order_['sells_num'], $add_data['kojin_num']);
//        var_dump($sells_data);

        // restock items
        foreach($sells_data as $line) {
            self::restock($line['list_num'], $line['buy_n']);
        }

        return self::recreateCart($order_);
    }


    /**
     * 注文がｌキャンセルされた後に、カートを再作成する
     *
     * @param array $order_ (webcollectリコード)
     * @return array
     */
    static function recreateCart($order_) {
        $add_data = self::readAdd($order_['add_num']);

        $sells_data = self::readSells($order_['sells_num'], $add_data['kojin_num']);
        //var_dump($add_data);

        // お客さん情報
        $kojin_num = '';
        $name_s = $add_data['name_s'];
        $name_n = $add_data['name_n'];
        $kana_s = $add_data['kana_s'];
        $kana_n = $add_data['kana_n'];
        $zip1 = $add_data['zip1'];
        $zip2 = $add_data['zip2'];
        $prf = $add_data['prf'];
        $city = $add_data['city'];
        $add1 = $add_data['add1'];
        $add2 = $add_data['add2'];
        $tel1 = $add_data['tel1'];
        $tel2 = $add_data['tel2'];
        $tel3 = $add_data['tel3'];
        $fax1 = $add_data['fax1'];
        $fax2 = $add_data['fax2'];
        $fax3 = $add_data['fax3'];
        $email = $add_data['email'];
        $pay_type = 4;

        // unused
        $point = '';
        $t_time = '';
        $r_point = '';
        $msg = '';
        $delivery = '';

        // cart details
        $cart = array(
            'items' => array(),
            'info' => "$kojin_num<>$name_s<>$name_n<>$kana_s<>$kana_n<>$email<>$zip1<>$zip2<>$prf<>$city<>$add1<>$add2<>$tel1<>$tel2<>$tel3<>$fax1<>$fax2<>$fax3<>$point<>$t_time<>$r_point<>$g_point<>$msg<>$pay_type<>$delivery<>"
        );

        foreach($sells_data as $line) {
            $cart['items'][$line['list_num']] = $line['buy_n'];
        }


        return $cart;
    }


    /**
     * 注文をキャンセルする時に、goodsテーブルのstockに追加
     *
     * @global db $db
     * @param int $list_num_
     * @param int $qty_
     */
    static function restock($list_num_, $qty_) {
        global $db;

        // get pluid from list table
        $goods_sql = "SELECT * FROM list WHERE list_num = $list_num_";
        $rs_goods = pg_query($db, $goods_sql);
        $goods = pg_fetch_assoc($rs_goods);
        $pluid = $goods['pluid'];

        // update goods table
        $update_sql = "UPDATE goods SET stock = stock + $qty_ WHERE pluid = '$pluid'";
        //die($update_sql);
        pg_query($update_sql);
    }


    /**
     * webcollectのエラーコードに対して、メッセージを出す
     *
     * @param int $settle_detail_
     * @return string
     */
    static function settleDetailText($settle_detail_) {
        $list = array(
            11 => "購入者都合エラー（カード情報に誤りがある場合等）",
            12 => "加盟店都合エラー（決済取消等）",
            13 => "決済機関都合エラー（決済機関から応答が無い場合、異常の応答を受けた場合等）",
            14 => "その他システムエラー",
            15 => "予約販売与信エラー（日付の設定に誤りがある場合等）",
            18 => "継続課金与信エラー",
        );

        return $list[$settle_detail_];
    }


    // MESSAGES


    static function messageSuccess($msg_) {
        $html = "<div class='webcollect-success'>$msg_</div>";

        return $html;
    }


    static function messageWarning($msg_) {
        $html = "<div class='webcollect-warning'>$msg_</div>";

        return $html;
    }


    static function messageError($msg_) {
        $html = "<div class='webcollect-error'>$msg_</div>";

        return $html;
    }

}