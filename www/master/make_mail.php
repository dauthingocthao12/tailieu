<?PHP

//	発送メール作成プログラム

	$sells_num=$_POST["sells_num"];
	$kojin_num=$_POST["kojin_num"];
	$unsou=$_POST["unsou"];
	$bangou=$_POST["bangou"];
	if (!$sells_num && !$kojin_num) { ERROR(); }

include "../../cone.inc";
include '../sub/array.inc';

	$sql  = "select hinban, title, price, buy_n, send, h_time, bargain, add_num from sells";
	$sql .= " where sells_num='$sells_num' AND hinban!='option'";
	$sql .= " ORDER BY hinban;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	for($i=0; $i<$count; $i++) {
		list($hinban, $title, $price, $buy_n, $send, $h_time, $bargain, $add_num) = pg_fetch_array($sql1,$i);

		if ($title != "option") {
##			list($k_year,$k_mon,$k_day) = split("-",$h_time);
			list($k_year,$k_mon,$k_day) = explode("-",$h_time);

			$coment = "";
			if ($send == 0) {
				$coment = "準備中";
			}
			elseif ($send == 1) {
				$coment = "発送日： $k_year" . "年$k_mon" . "月$k_day" . "日";
			}
			elseif ($send == 2) {
				$coment = "キャンセル";
			}
			elseif ($send == 3) {
				$coment = "発送予定日： $k_year" . "年$k_mon" . "月$k_day" . "日";
			}

			$goods_a .= <<<ALPHA
商品番号：$hinban 
商品名：$title
数量：$buy_n 
$coment
--------------------------------------------------------

ALPHA;

		}
	}

	$sql = "SELECT * FROM option WHERE sells_num='$sells_num' ORDER BY hinban;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	for($i=0; $i<$count; $i++) {
		list($option_num_, $sells_num_, $kojin_num_, $hinban_, $title_, $seban_l_, $seban_num_, $seban_price_, $sename_l_, $sename_name_, $sename_price_, $muneban_l_, $muneban_num_, $muneban_price_, $pant_l_, $pant_num_, $pant_price_, $bach_l_, $bach_name_, $bach_price_, $send_, $h_time_) = pg_fetch_array($sql1,$i);

##		list($k_year,$k_mon,$k_day) = split("-",$h_time_);
		list($k_year,$k_mon,$k_day) = explode("-",$h_time_);
		$coment = "";
		if ($send_ == 0) {
			$coment = "準備中";
		}
		elseif ($send_ == 1) {
			$coment = "発送日： $k_year" . "年$k_mon" . "月$k_day" . "日";
		}
		elseif ($send_ == 2) {
			$coment = "キャンセル";
		}
		elseif ($send_ == 3) {
			$coment = "発送予定日： $k_year" . "年$k_mon" . "月$k_day" . "日";
		}

		$goods_b = <<<ALPHA
マーキング
マーキング商品番号：$hinban_
マーキング商品名：$title_

ALPHA;

//	背番号
		if ($seban_l_) {
			$moji_num = strlen($seban_num_);
			$goods_b .= <<<ALPHA
背番号：$SEBAN_N[$seban_l_]
番号：$seban_num_

ALPHA;
		}

//	背ネーム
		if ($sename_l_) {
			$sename_name_ = str_replace('\\', '', $sename_name_);
			$goods_b .= <<<ALPHA
背ネーム：$SENAME_N[$sename_l_]
ネーム：$sename_name_

ALPHA;
		}

//	胸番号
		if ($muneban_l_) {
			$moji_num = strlen($muneban_num_);
			$goods_b .= <<<ALPHA
胸番号：$MUNEBAN_N[$muneban_l_]
番号：$muneban_num_

ALPHA;
		}

//	パンツ番号
		if ($pant_l_) {
			$moji_num = strlen($pant_num_);
			$goods_b .= <<<ALPHA
パンツ番号：$PANT_N[$pant_l_]
番号：$pant_num_

ALPHA;
		}

//	バッジ
		if ($bach_l_) {
			$goods_b .= "バッジ：$BACH_N[$bach_l_]\n";
		}

		$goods_a .= <<<ALPHA
$goods_b$coment
--------------------------------------------------------

ALPHA;

	$goods_b = "";

	}

	$sql = "select name_s, name_n from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n) = pg_fetch_array($sql1,0);
	$name = "$name_s $name_n";

	if (!$add_num) {
		$sql  = "select add_num from sells where sells_num='$sells_num' AND sells_num!='';";
		$sql1 = pg_exec($conn_id,$sql);
		list($add_num) = pg_fetch_array($sql1,0);
	}

	$sql =  "SELECT name_s, name_n, kana_s, kana_n, zip1, zip2, prf, city, add1, add2, tel1, tel2, tel3, fax1, fax2, fax3, email FROM add" .
			" WHERE add_num='$add_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email) = pg_fetch_array($sql1,0);
	$TEL = "$tel1" . "-" . "$tel2" . "-" . "$tel3";
	$FAX = "$fax1" . "-" . "$fax2" . "-" . "$fax3";

//	$goods_aは、商品データー

	//	発送会社情報
	$UNSOU = array('佐川急便','ヤマト運輸','日本郵便','西濃運輸');	//	add ookawara 2011/05/23 日本郵便	add ookawara 2011/07/12 西濃運輸
	$UN_URL = array('http://k2k.sagawa-exp.co.jp/p/sagawa/web/okurijoinput.jsp','http://toi.kuronekoyamato.co.jp/cgi-bin/tneko?init','http://tracking.post.japanpost.jp/service/numberSearch.do?searchKind=S002','http://track.seino.co.jp/kamotsu/GempyoNoShokai.do');	//	add ookawara	2010/04/09	//	add ookawara 2011/05/23
//	$UN_URL = array('http://k2k.sagawa-exp.co.jp/cgi-bin/SagawaWeb.pcgi','http://toi.kuronekoyamato.co.jp/cgi-bin/tneko?init');	//	del ookawara 2010/04/09
	$un_office = $UNSOU[$unsou];
	$bangou = trim($bangou);
#	$bangou = mb_convert_kana($bangou,"a","EUC-JP");
	$bangou = mb_convert_kana($bangou,"a","UTF-8");

	$msg = <<<ALPHA
$name 様この度はご注文頂き、ありがとうございました。
下記のご注文の商品を発送しましたので、ご連絡致します。
##########################################################
運送会社：$un_office
伝票番号：$bangou
お問い合わせURL：
$UN_URL[$unsou]

ご不在の際は $un_office の不在票が入りますのでお手数ですが
不在票に記載されております営業所にご連絡して下さい。
##########################################################

ご注文商品
----------------------------------------------------------
$goods_a
送り先情報
----------------------------------------------------------

氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

住所
　〒$zip1-$zip2
　$PRF_N[$prf] $city $add1 $add2

電話番号
　$TEL

FAX番号
　$FAX

メールアドレス
　$email

##########################################################

$m_footer

ALPHA;

$subject = "商品を発送致しました。 -ネイバーズスポーツ-";

	echo <<<ALPHA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>発送メール</TITLE>
</HEAD>
<BODY>
<FORM action="./mail_send.php" method="POST">
<INPUT type="hidden" name="check" value="check">
<TABLE border="0" cellpadding="4" cellspacing="1" bgcolor="#666666">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc">お客様名</TD>
      <TD bgcolor="#ffffff">$name (No.$kojin_num)</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">ご注文番号</TD>
      <TD bgcolor="#ffffff">$sells_num</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">メールアドレス</TD>
      <TD bgcolor="#ffffff"><INPUT type="text" size="60" name="email" value="$email"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">メールタイトル</TD>
      <TD bgcolor="#ffffff"><INPUT size="60" type="text" name="subject" value="$subject"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc">メール本文</TD>
      <TD bgcolor="#ffffff"><TEXTAREA rows="40" cols="80" name="msg">$msg</TEXTAREA></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff" colspan="2" align="center"><INPUT type="submit" value="送信"><INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
</BODY>
</HTML>

ALPHA;

	pg_close($conn_id);


function ERROR() {

	echo <<<ALPHA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>発送メール</TITLE>
</HEAD>
<BODY>
<BR>
<B><FONT color="#ff0000">エラー</FONT></B><BR>
お客様情報が読み込めません。<BR>
発注状況の画面から作業して下さい。
</BODY>
</HTML>

ALPHA;

exit;

}

?>
