<?PHP

//	チェック
function check($KAGOS,$OPTIONS) {

	$customer = $_SESSION['customer'];
	$opt = $_SESSION['opt'];

	if (!$KAGOS && $customer) {
		$KAGO = explode("<>",$customer);

		unset($KAGOS);
		unset($CHECK);
		foreach ($KAGO AS $val) {
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if (!$title_) {
				continue;
			} else {
				$KAGOS[] = "$val";
				$CHECK[$hinban_] = $hinban_;
			}
		}

		if (!$KAGOS) {
			unset($_SESSION['customer']);
		}
	}

	if (!$OPTIONS && $opt) {
##		$OPTIONS_L = split("<>",$opt);
		$OPTIONS_L = explode("<>",$opt);

		unset($OPTIONS);
		$op_max_num = 0;
		foreach ($OPTIONS_L AS $val) {
			list($op_num,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);
			if ($op_max_num < $op_num) {
				$op_max_num = $op_num;
			}
			if (!$hinban_) {
				continue;
			} elseif ($hinban_ != "mochikomi" && !$CHECK[$hinban_]) {
				continue;
			} else {
				$OPTIONS[] = "$val";
			}
		}

		if (!$OPTIONS) {
			unset($_SESSION['opt']);
		}
	}

	return array($KAGOS,$OPTIONS);

}

//	追加
function add(&$KAGOS, &$OPTIONS, &$ERROR) {

	$hinban = $_POST['hinban'];
	$title = $_POST['title'];
	$kakaku = $_POST['kakaku'];
	$code = $_POST['code'];
	$name = $_POST['name'];
	$size = $_POST['size'];

	if (!$hinban && !$title && $code && $name && $size) {
		$hinban = "$code" . "-$size";
		$title = "$name/$size";
	}

	//	リファラを取得
	$refere = $_SESSION['refere'];

	if ($hinban && $title && $kakaku) {
		$refere_arreay = parse_url($refere);
		//	リファラからドメインを取得してネイバーズスポーツ内のリンクからの注文か確認、そうでなければ注文をキャンセルする
		$refere_host = $refere_arreay['host'];
		$flags = 0;
		if (preg_match("/futboljersey.com/",$refere_host))  { $flags = 1;}
		if ($flags == 0) {
			$hinban = ""; $title = ""; $kakaku = "";
		}
	}

	$customer = $_SESSION['customer'];

//	unset($ERROR);
$ERROR = array();
	if (!$hinban) {
		$ERROR[] = "商品番号が入力されておりません。";
	}
	if (!$title) {
		$ERROR[] = "商品名が入力されておりません。";
	}
	if (!$kakaku) {
		$ERROR[] = "商品価格が入力されておりません。";
	}
	if ($title) {
		$title = preg_replace("/\・/", "/", $title);
		$title = preg_replace("/\･/", "/", $title);
	}

	//	ログ取得
	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) { $host = $ip; }

	$sql  = "INSERT INTO cart_access" .
			" (code,name,kakaku,ip,host,time)" .
			" VALUES('$hinban','$title','$kakaku','$ip','$host',NOW());";
	$sql1 = pg_query(DB,$sql);

	//	特価商品のカテゴリーの商品だった場合1点しか購入出来ないようにする。
	//	2014/08/01	ookawara
	$tokka_flg = check_tokka($hinban);

	//unset($KAGOS);			//	del yoshizawa 2013/11/22
	if (!$ERROR) {
		$KAGOS = array();	//	add yoshizawa 2013/11/22 エラー時に商品情報がなくなってしまうので分岐内に組み込む
		$KAGO = explode("<>",$customer);
		$flag = 0;
		foreach ($KAGO AS $val) {
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if ($title_ == "") {
				continue;
			} elseif ($hinban_ == $hinban) {
				//	add ookawara 2014/08/01 start
				if ($tokka_flg > 0) {
					$num_ = 1;
					//$ERROR[] = "特価商品は、1点のみ購入可能です。";							//	del ookawara 2014/12/24
					$ERROR[] = "特価商品、または、限定入荷一点限りは、1点のみ購入可能です。";	//	add ookawara 2014/12/24
				} else {
				//	add ookawara 2014/08/01 end
					++$num_;
				}	//	add ookawara 2014/08/01 start

				$KAGOS[] = "$hinban_::$title_::$kakaku_::$num_::";
				$flag = 1;
			} else {
				$KAGOS[] = "$val";
			}
		}
		if ($flag == 0) {
			$KAGOS[] = "$hinban::$title::$kakaku::1::";
		}
	}
}



//	特価商品チェック
//	2014/08/01	ookawara
function check_tokka($hinban) {

	//	サイズを抜き取る
	$HINBAN = explode("-", $hinban);
	$last_cnt = count($HINBAN) - 1;
	$size = "-".$HINBAN[$last_cnt];
	$code = preg_replace("/$size/","",$hinban);

	$tokka_flg = 0;
	if ($hinban) {
		$sql  = "SELECT COUNT(cate.*) AS tokka_flg FROM category cate".
				" INNER JOIN goods gd ON gd.g_num=cate.g_num".
				" WHERE gd.code='".$code."'".
				//" AND cate.cate1=23;";		//	del 2014/12/24	ookawara
				" AND cate.cate1 IN (23,24);";	//	add 2014/12/24	ookawara
		if ($result = pg_query(DB, $sql)) {
			$list = pg_fetch_array($result);
			$tokka_flg = $list['tokka_flg'];
		}
	}

	return $tokka_flg;
}



//	変更
function hen(&$KAGOS,&$ERROR) {
//echo('hen通過<br>');

	$hinban = $_POST['hinban'];

	$customer = $_SESSION['customer'];

	$num = $_POST['num'];
#	$num = mb_convert_kana($num,n,"EUC-JP");
	$num = mb_convert_kana($num,n,"UTF-8");
	if (preg_match("/[^0-9]/",$num)) {
		$ERROR[] = "注文変更数が不正です。";
	}

	//	特価商品のカテゴリーの商品だった場合1点しか購入出来ないようにする。
	//	2014/08/01	ookawara
	$tokka_flg = check_tokka($hinban);

	if (!$ERROR) {
		if ($num <= 0) {
			del($KAGOS);
		}else {
			$KAGOS = array();
			$KAGO = explode("<>",$customer);
			$flag = 0;
			foreach ($KAGO AS $val) {
				list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
				if ($title_ == "") {
					continue;
				} elseif ($hinban_ == $hinban) {
					//	add ookawara 2014/08/01 start
					if ($tokka_flg > 0) {
						if ($num > 1) {
							//$ERROR[] = "特価商品は、1点のみ購入可能です。";							//	del ookawara 2014/12/24
							$ERROR[] = "特価商品、または、限定入荷一点限りは、1点のみ購入可能です。";	//	add ookawara 2014/12/24
						}
						$num = 1;
					}
					//	add ookawara 2014/08/01 end

					$KAGOS[] = "$hinban_::$title_::$kakaku_::$num::";
				} else {
					$KAGOS[] = "$val";
				}
			}
		}
	}

}

//	削除
function del(&$KAGOS) {
//echo('del通過<br>');

	$hinban = $_POST['hinban'];

	$KAGOS = array();
	$customer = $_SESSION['customer'];
	$KAGO = explode("<>",$customer);
	foreach ($KAGO AS $val) {
		list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
		if ($hinban_ == $hinban || $title_ == "") {
			continue;
		} else {
			$KAGOS[] = "$val";
		}
	}
	if (!$KAGOS) {
		unset($_SESSION['customer']);
		unset($_SESSION['addr']);
	}

}

//	削除マーキング
function del_op(&$OPTIONS) {
//echo('del_op通過<br>\n');

	$op_num = trim($_POST['op_num']);

	$OPTIONS = array();
	$opt = $_SESSION['opt'];
	$OPTIONS_L = explode("<>",$opt);
	foreach ($OPTIONS_L AS $val) {
		list($op_num_,$hinban_) = explode("::",$val);
		if ($op_num == $op_num_ || $hinban_ == "") {
			continue;
		} else {
			$OPTIONS[] = "$val";
		}
	}
	if (!$OPTIONS) {
		unset($_SESSION['opt']);
	}

}

//	商品なし
function cago_empty_html(){

	$html = "";

	$DEL_INPUTS['CAGOGOODS'] = 1;

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("cago.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	商品あり
function cago_goods_html($KAGOS , $OPTIONS , $ERROR){

	global	$TAX_ , $mochi_pri , $PHP_SELF , $SEBAN_P_N , $SENAME_P_N , $MUNEBAN_P_N , $PANT_P_N , $BACH_P_N ,
			$waribiki , $waribiki2 , $wa_member , $DISCOUNT_C , $DISCOUNT , $SOURYOU_MURYOU;

/*
	//	PayPalセッション削除
	//	del ookawara	2014/05/13
	unset($_SESSION['paypal_list']);
	unset($_SESSION['payer_id']);
	unset($_SESSION['curl_error_no']);
	unset($_SESSION['curl_error_msg']);
	unset($_SESSION['nvpReqArray']);
	unset($_SESSION['TOKEN']);
	unset($_SESSION['currencyCodeType']);
	unset($_SESSION['PaymentType']);
	unset($_SESSION['Payment_Amount']);
*/

	$html = "";

/*
	//	del ookawara 2014/05/13
	if ($_POST['action'] != "back"){
		unset($_SESSION['addr']);
	}
*/

	//	アンドロイド対策
	//	add ookawara 2014/05/13
	if ($_POST['action'] != "back" && $_SESSION['cago_form'] == "back") {
		unset($_SESSION['addr']);
	}
	if ($_SESSION['cago_form'] == "back") {
		unset($_SESSION['cago_form']);
	}


	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$addr = $_SESSION['addr'];



	//	商品表示
	unset($customer);
	$price_all = 0;
	$souryou_muryou_flag = 0;
	if ($KAGOS) {

		// 2022/12/5 レスポンシブ対応につき、コメントアウト uenishi
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"syouhin\">\n";
		// $html .= "			商品番号\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"tanka\" rowspan=\"2\">\n";
		// $html .= "			単価\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"suuryou\" rowspan=\"2\">\n";
		// $html .= "			数量\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"syoukei\" rowspan=\"2\">\n";
		// $html .= "			小計\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"sakuzyo\" rowspan=\"2\">\n";
		// $html .= "			削除\n";
		// $html .= "		</th>\n";
		// $html .= "	</tr>\n";
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"sakuzyo\">商品名</th>\n";
		// $html .= "	</tr>\n";


		foreach ($KAGOS AS $val) {
			$syoukei = 0;
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if ($hinban_ == "") {
				continue;
			} else {
				$customer .= $val."<>";
			}
			$_SESSION['customer'] = $customer;	//	$_SESSION['customer']が生成される

			//	送料無料チェック
			$hinban_id = "";
			$GDNM = explode("-",$hinban_);
			$gdnm_max = count($GDNM) - 1;
			for ($i=0; $i<$gdnm_max; $i++) {
				if ($hinban_id) {
					$hinban_id .= "-";
				}
				$hinban_id .= $GDNM[$i];
			}

			if ($SOURYOU_MURYOU[$hinban_id]) {
				$souryou_muryou_flag = 1;
			}

			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $num_;
			$price_all = $price_all + $syoukei;
			$kakaku_h = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			// 2022/12/5 レスポンシブ対応につき、コメントアウト uenishi
			// $html .= "	<tr>\n";
			// $html .= "		<td class=\"cago_title\">\n";
			// $html .= "			".$hinban_."\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">\n";
			// $html .= "			".$kakaku_h." 円\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">\n";
			// $html .= "			<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			// $html .= "				<input type=\"hidden\" name=\"mode\" />\n";
			// $html .= "				<input type=\"hidden\" name=\"action\" value=\"hen\" />\n";
			// $html .= "				<input type=\"hidden\" name=\"hinban\" value=\"".$hinban_."\" />\n";
			// $html .= "				<input size=\"4\" type=\"text\" name=\"num\" value=\"".$num_."\" /><br />\n";
			// $html .= "				<input type=\"submit\" value=\"数量変更\" />\n";
			// $html .= "			</form>\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">".$syoukei." 円</td>\n";
			// $html .= "		<td rowspan=\"2\">\n";
			// $html .= "			<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			// $html .= "				<input type=\"hidden\" name=\"mode\" />\n";
			// $html .= "				<input type=\"hidden\" name=\"action\" value=\"del\" />\n";
			// $html .= "				<input type=\"hidden\" name=\"hinban\" value=\"".$hinban_."\" />\n";
			// $html .= "				<input type=\"submit\" value=\"削除\" />\n";
			// $html .= "			</form>\n";
			// $html .= "		</td>\n";
			// $html .= "	</tr>\n";
			// $html .= "	<tr>\n";
			// $html .= "		<td class=\"cago_title\">".$title_."</td>\n";
			// $html .= "	</tr>\n";

			$html .= "<div class=\"box-outline\">\n";
			$html .= "	<h4 class=\"order-headline text-center\">\n";
			$html .= "		オーダー内容\n";
			$html .= "	</h4>\n";
			$html .= "		<div class=\"box-row\">\n";
			$html .= "			<div class=\"box-content box-item\">\n";
			$html .= "				<span class=\"bold\">商品</span>\n";
			$html .= "				<div>\n";
			$html .= "					<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			$html .= "						<input type=\"hidden\" name=\"mode\" />\n";
			$html .= "						<input type=\"hidden\" name=\"action\" value=\"del\" />\n";
			$html .= "						<input type=\"hidden\" name=\"hinban\" value=\"".$hinban_."\" />\n";
			$html .= "						<input type=\"submit\" value=\"削除\" class=\"btn-standard delete-button\"/>\n";
			$html .= "					</form>\n";
			$html .= "				</div>\n";
			$html .= "			</div>\n";
			$html .= "	 		<div class=\"box-content\">\n";
			$html .= "				<div class=\"product-detail\">\n";
			$html .= "					<div class=\"product-info\">\n";
			$html .= "						<div class=\"box-row fixed-row\">\n";
			$html .= "						 	<div class=\"product-detail-name\">商品名：　</div>\n";
			$html .= "                       	<span class=\"bold\">".$title_."</span>\n";
			$html .= "						</div>\n";
			$html .= "						<div class=\"box-row fixed-row\">\n";
			$html .= "		    				<div class=\"product-detail-name\">商品番号：　</div>\n";
			$html .= "		    				<span class=\"bold\">".$hinban_."</span>\n";
			$html .= "						</div>\n";
			$html .= "						<div class=\"box-row fixed-row\">\n";
			$html .= "							<div class=\"product-detail-name\">単価：　</div>\n";
			$html .= "							<span class=\"bold\">".$kakaku_h." 円</span>\n";
			$html .= "						</div>\n";
			$html .= "					</div>\n";
			$html .= "					<div class=\"amount\">\n";
			$html .= "						<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			$html .= "							<div class=\"box-col-2\">\n";
			$html .= "								<input type=\"hidden\" name=\"mode\" />\n";
			$html .= "								<input type=\"hidden\" name=\"action\" value=\"hen\" />\n";
			$html .= "								<input type=\"hidden\" name=\"hinban\" value=\"".$hinban_."\" />\n";
			$html .= "								<span>数量：</span><input type=\"number\" name=\"num\" value=\"".$num_."\" class=\"change-quantity\" /><br />\n";
			$html .= "								<input type=\"submit\" value=\"数量変更\" class=\"btn-standard change-button\"/>\n";
			$html .= "							</div>\n";
			$html .= "						</form>\n";
			$html .= "						<div class=\"sub-total\">\n";
			$html .= "							小計：　<span class=\"bolder\">".$syoukei." 円</span>\n";
			$html .= "						</div>\n";
			$html .= "					</div>\n";
			$html .= "				</div>\n";
			$html .= "			</div>\n";
			$html .= "	</div>\n";
			$html .= "</div>\n";

		}
	}

	//	マーキングオーダー表示
	if (!$KAGOS && $OPTIONS) {

		// 2022/12/2 レスポンシブ対応につき、コメントアウト uenishi
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"syouhin\">\n";
		// $html .= "			商品名\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"tanka\">\n";
		// $html .= "			単価\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"suuryou\">\n";
		// $html .= "			数量\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"syoukei\">\n";
		// $html .= "			小計\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"syoukei\">\n";
		// $html .= "		</th>\n";
		// $html .= "	</tr>\n";
	}
	if ($OPTIONS) {

		unset($opt);
		foreach ($OPTIONS AS $val) {
			$syoukei = 0;
			list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);

			if ($hinban_ == "") {
				continue;
			} else {
				$opt .= $val."<>";
			}
			$_SESSION['opt'] = $opt;	//	削除後に商品データ（配列）をもとに$_SESSION['opt']を書き換える

			// 2022/12/2 レスポンシブ対応につき、コメントアウト uenishi
			// $marking_html .= "<tr>\n";
			// $marking_html .= "	<td class=\"cago_title cago_mar\" colspan=\"4\">\n";
			// $marking_html .= "		マーキング 商品名：".$title_;
			// $marking_html .= "	</td>\n";
			// $marking_html .= "	<td class=\"cago_mar\">\n";
			// $marking_html .= "		<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			// $marking_html .= "			<input type=\"hidden\" name=\"mode\" />\n";						//	update 2013/11/26 yoshizawa value=\"del_op\"削除
			// $marking_html .= "			<input type=\"hidden\" name=\"action\" value=\"del_op\" />\n";	//	add yoshizawa 2013/11/25
			// $marking_html .= "			<input type=\"hidden\" name=\"op_num\" value=\"".$op_num_."\" />\n";
			// $marking_html .= "			<input type=\"submit\" value=\"削除\" />\n";
			// $marking_html .= "		</form>\n";
			// $marking_html .= "	</td>\n";
			// $marking_html .= "</tr>\n";


			$marking_html .= "<div class=\"box-outline\">\n";
			$marking_html .= "	<h4 class=\"order-headline text-center\">オーダー内容</h4>\n";
			$marking_html .= "		<div class=\"box-row-nogap\">\n";
			$marking_html .= "			<div class=\"box-content box-item\">\n";
			$marking_html .= "				<span class=\"bold\">マーキング商品</span>\n";
			$marking_html .= "				<form action=\"".$PHP_SELF."\" method=\"post\">\n";
			$marking_html .= "					<input type=\"hidden\" name=\"mode\" />\n";
			$marking_html .= "					<input type=\"hidden\" name=\"action\" value=\"del_op\" />\n";
			$marking_html .= "					<input type=\"hidden\" name=\"op_num\" value=\"".$op_num_."\" />\n";
			$marking_html .= "					<input type=\"submit\" value=\"削除\" class=\"btn-standard delete-button\"/>\n";
			$marking_html .= "				</form>\n";
			$marking_html .= "			</div>\n";
			$marking_html .= "		<div class=\"box-stretch\">\n";
			$marking_html .= "		    <div class=\"box-column\">\n";
			$marking_html .= "				<div class=\"box-content\">\n";
			$marking_html .= "               	<span class=\"bold text-center\">".$title_."</span>\n";
			$marking_html .= "				</div>\n";
			$marking_html .= "				<div class=\"breakdown\">\n";
			$marking_html .= "					<span>番号、ネームなど</span>\n";
			$marking_html .= "					<span>単価</span>\n";
			$marking_html .= "					<span>数量</span>\n";
			$marking_html .= "					<span>小計<span>\n";
			$marking_html .= "				</div>\n";





			//	持ち込み手数料
			if ($hinban_ == "mochikomi") {
				$kakaku_ = $mochi_pri;
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">持ち込み手数料</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>1</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "		<div class=\"box-row box-content content-grid fixed-row\">\n";
				$marking_html .= "			<span>持ち込み手数料</span>\n";
				$marking_html .= "			<span>".$kakaku_." 円</span>\n";
				$marking_html .= "			<span>1</span>\n";
				$marking_html .= "			<span class=\"bold\">".$syoukei."円<span>\n";
				$marking_html .= "		</div>\n";
			}

			//	背番号
			if ($seban_l_) {
				$moji_num = strlen($seban_num_);				//文字列のながさ
				$kakaku_ = $SEBAN_P_N[$seban_l_];				//タイプの価格
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);	//端数の切り捨て
				$syoukei = $kakaku_ * $moji_num;				//番号の数×値段
				$price_all = $price_all + $syoukei;				//＋$syoukei
				$kakaku_ = number_format($kakaku_);				//数字を千位毎にグループ化してフォーマットする
				$syoukei = number_format($syoukei);				//数字を千位毎にグループ化してフォーマットする

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">背番号 ".$SEBAN_N[$seban_l_]." 番号：".$seban_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "	<div class=\"box-row box-content content-grid fixed-row\">\n";
				$marking_html .= "		<span>背番号".$SEBAN_N[$seban_l_]."：".$seban_num_."</span>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "		<span>".$moji_num."</span>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
			}

			//	背ネーム
			if ($sename_l_) {
				$sename_name_ = str_replace('\\', '', $sename_name_);
				$sename_name_m = str_replace(' ', '', $sename_name_);
				$moji_num = strlen($sename_name_m);
				$kakaku_ = $SENAME_P_N[$sename_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">背ネーム ".$SENAME_N[$sename_l_]." ネーム：".$sename_name_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "	<div class=\"box-row box-content content-grid fixed-row\">\n";
				$marking_html .= "		<span>背ネーム ".$SENAME_N[$sename_l_]."：".$sename_name_."</span>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "		<span>".$moji_num."</span>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
			}

			//	胸番号
			if ($muneban_l_) {
				$moji_num = strlen($muneban_num_);
				$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">胸番号 ".$MUNEBAN_N[$muneban_l_]." 番号：".$muneban_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "	<div class=\"box-row box-content content-grid fixed-row\">\n";
				$marking_html .= "		<span>胸番号 ".$MUNEBAN_N[$muneban_l_]."：".$muneban_num_."</span>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "		<span>".$moji_num."</span>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
			}

			//	パンツ番号
			if ($pant_l_) {
				$moji_num = strlen($pant_num_);
				$kakaku_ = $PANT_P_N[$pant_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">パンツ番号 ".$PANT_N[$pant_l_]." 番号：".$pant_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "	<div class=\"box-row box-content content-grid fixed-row\">\n";
				$marking_html .= "		<span>パンツ番号 ".$PANT_N[$pant_l_]."：".$pant_num_."</span>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "		<span>".$moji_num."</span>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
			}

			//	バッジ
			if ($bach_l_) {
				$kakaku_ = $BACH_P_N[$bach_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">バッジ ".$BACH_N[$bach_l_]."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>1</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "	<td></td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "				<div class=\"box-row box-content box-space content-grid fixed-row\">\n";
				$marking_html .= "					<span>バッジ ".$BACH_N[$bach_l_]."</span>\n";
				$marking_html .= "					<span>".$kakaku_." 円</span>\n";
				$marking_html .= "					<span>".$moji_num."</span>\n";
				$marking_html .= "					<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "				</div>\n";
				$marking_html .= "			</div>\n";
				$marking_html .= "		</div>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";

			}
		}
	}



	$idpass = $_SESSION['idpass'];
	$addr = $_SESSION['addr'];
//$waribiki = 10;
//$waribiki2 = 30;
//$DISCOUNT_C = 1;
	//	割引表示
	if ($idpass && $waribiki > 0) {

		list($id,$pass) = explode("<>",$idpass);
		$sql =  "SELECT kojin_num FROM ".T_KOJIN.
				" WHERE email='".$id."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000';";
		$sql1 = pg_query(DB,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 1) {
			list($kojin_num) = pg_fetch_array($sql1,0);
		}
		if ($kojin_num <= $wa_member) {
			$nebiki = $price_all * $waribiki / 100;
			$price_all = $price_all - $nebiki;
			// 合計金額に小数点以下0.5が入るとnumber_formatで四捨五入されて実計算と誤差が出る
			// 表示する値引き金額を切り捨てて整数とする
			$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
			$nebiki = number_format($nebiki);

			//$DEL_INPUTS['WARIBIKIDEL'] = 1;		//	会員割引削除
			$DEL_INPUTS['WARIBIKI2DEL'] = 1;		//	非会員割引削除
			$DEL_INPUTS['PAERSENTDEL'] = 1;			//	購入金額割引削除
		}

	} elseif (!$idpass && $waribiki2 > 0) {

		$nebiki = $price_all * $waribiki2 / 100;
		$price_all = $price_all - $nebiki;
		// 合計金額に小数点以下0.5が入るとnumber_formatで四捨五入されて実計算と誤差が出る
		// 表示する値引き金額を切り捨てて整数とする
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

			$DEL_INPUTS['WARIBIKIDEL'] = 1;			//	会員割引削除
			//$DEL_INPUTS['WARIBIKI2DEL'] = 1;		//	非会員割引削除
			$DEL_INPUTS['PAERSENTDEL'] = 1;			//	購入金額割引削除

	} elseif ($DISCOUNT_C == 1) {

		unset($nebiki);
		$DISCOUNT = array_reverse($DISCOUNT);
		foreach ($DISCOUNT AS $VAL) {
			$totalprice_ = $VAL[0];
			$paersent_ = $VAL[1];
			if ($price_all > $totalprice_) {
				$paersent = $paersent_;
				break;
			}
		}
		$nebiki = $price_all * $paersent / 100;
		$price_all = $price_all - $nebiki;
		// 合計金額に小数点以下0.5が入るとnumber_formatで四捨五入されて実計算と誤差が出る
		// 表示する値引き金額を切り捨てて整数とする
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		if ($nebiki) {
			$DEL_INPUTS['WARIBIKIDEL'] = 1;			//	会員割引削除
			$DEL_INPUTS['WARIBIKI2DEL'] = 1;		//	非会員割引削除
			//$DEL_INPUTS['PAERSENTDEL'] = 1;		//	購入金額割引削除
		}

	} else {
		$DEL_INPUTS['WARIBIKIDEL'] = 1;				//	会員割引削除
		$DEL_INPUTS['WARIBIKI2DEL'] = 1;			//	非会員割引削除
		$DEL_INPUTS['PAERSENTDEL'] = 1;				//	購入金額割引削除
	}

	$price_all = number_format($price_all);

	if ($souryou_muryou_flag == 1) {
		$DEL_INPUTS['SOURYOUARI'] = 1;			//	送料あり削除
	} else {
		$DEL_INPUTS['SOURYOUMURYOU'] = 1;		//	送料無料削除
	}

	if($idpass || $addr) {
		if($idpass){
			$DEL_INPUTS['TOUROKU'] = 1;			//	会員登録オススメ
		}
		$DEL_INPUTS['KOUNYUU2'] = 1;			//	購入(非会員)削除
	} else {
		$DEL_INPUTS['KOUNYUU1'] = 1;			//	購入(会員)削除
	}



	//	リファラを取得
	$refere = $_SESSION['refere'];

	$DEL_INPUTS['CAGOEMPTY'] = 1;						//	商品なしメッセージ削除

	$INPUTS['ERRORMSG'] = $error_html;					//	エラーメッセージ
	$INPUTS['REFERE'] = $refere;						//	「買い物をつづける」ボタンにリファラをセット
	$INPUTS['GOODS'] = $html;							//	商品表示
	$INPUTS['MARKING'] = $marking_html;					//	マーキング商品表示
	$INPUTS['WARIBIKI'] = $waribiki;					//	会員割引率
	$INPUTS['WARIBIKI2'] = $waribiki2;					//	非会員割引率
	$INPUTS['PAERSENT'] = $paersent;					//	購入金額割引率
	$INPUTS['NEBIKI'] = $nebiki;						//	値引き額
	$INPUTS['PRICEALL'] = $price_all;					//	合計金額

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("cago.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	入力フォームエラーチェック
function cago_check(&$ERROR){

	//	ユーザー情報取得
	$name_s = $_POST['name_s'];								//	姓
#	$name_s = mb_convert_kana($name_s, "asKV", "EUC-JP");
	$name_s = mb_convert_kana($name_s, "asKV", "UTF-8");
	$name_s = trim($name_s);
	$name_n = $_POST['name_n'];								//	名
#	$name_n = mb_convert_kana($name_n, "asKV", "EUC-JP");
	$name_n = mb_convert_kana($name_n, "asKV", "UTF-8");
	$kana_s = $_POST['kana_s'];								//	姓：ふりがな
#	$kana_s = mb_convert_kana($kana_s, "ascHV", "EUC-JP");
	$kana_s = mb_convert_kana($kana_s, "ascHV", "UTF-8");
	$kana_n = $_POST['kana_n'];								//	名：ふりがな
#	$kana_n = mb_convert_kana($kana_n, "ascHV", "EUC-JP");
	$kana_n = mb_convert_kana($kana_n, "ascHV", "UTF-8");
	$zip1 = $_POST['zip1'];									//	郵便番号1
#	$zip1 = mb_convert_kana($zip1, "as", "EUC-JP");
	$zip1 = mb_convert_kana($zip1, "as", "UTF-8");
	$zip2 = $_POST['zip2'];									//	郵便番号2
#	$zip2 = mb_convert_kana($zip2, "as", "EUC-JP");
	$zip2 = mb_convert_kana($zip2, "as", "UTF-8");
	$prf = $_POST['prf'];									//	都道府県
	$city = $_POST['city'];									//	市区町村名
#	$city = mb_convert_kana($city, "asKV", "EUC-JP");
	$city = mb_convert_kana($city, "asKV", "UTF-8");
	$add1 = $_POST['add1'];									//	所番地
#	$add1 = mb_convert_kana($add1, "asKV", "EUC-JP");
	$add1 = mb_convert_kana($add1, "asKV", "UTF-8");
	$add2 = $_POST['add2'];									//	マンション名など
#	$add2 = mb_convert_kana($add2, "asKV", "EUC-JP");
	$add2 = mb_convert_kana($add2, "asKV", "UTF-8");
	$tel1 = $_POST['tel1'];									//	電話番号1
#	$tel1 = mb_convert_kana($tel1, "as", "EUC-JP");
	$tel1 = mb_convert_kana($tel1, "as", "UTF-8");
	$tel2 = $_POST['tel2'];									//	電話番号2
#	$tel2 = mb_convert_kana($tel2, "as", "EUC-JP");
	$tel2 = mb_convert_kana($tel2, "as", "UTF-8");
	$tel3 = $_POST['tel3'];									//	電話番号3
#	$tel3 = mb_convert_kana($tel3, "as", "EUC-JP");
	$tel3 = mb_convert_kana($tel3, "as", "UTF-8");
	$kei1 = $_POST['kei1'];									//	携帯番号1
#	$kei1 = mb_convert_kana($kei1, "as", "EUC-JP");
	$kei1 = mb_convert_kana($kei1, "as", "UTF-8");
	$kei2 = $_POST['kei2'];									//	携帯番号2
#	$kei2 = mb_convert_kana($kei2, "as", "EUC-JP");
	$kei2 = mb_convert_kana($kei2, "as", "UTF-8");
	$kei3 = $_POST['kei3'];									//	携帯番号3
#	$kei3 = mb_convert_kana($kei3, "as", "EUC-JP");
	$kei3 = mb_convert_kana($kei3, "as", "UTF-8");
	$fax1 = $_POST['fax1'];									//	FAX1
#	$fax1 = mb_convert_kana($fax1, "as", "EUC-JP");
	$fax1 = mb_convert_kana($fax1, "as", "UTF-8");
	$fax2 = $_POST['fax2'];									//	FAX2
#	$fax2 = mb_convert_kana($fax2, "as", "EUC-JP");
	$fax2 = mb_convert_kana($fax2, "as", "UTF-8");
	$fax3 = $_POST['fax3'];									//	FAX3
#	$fax3 = mb_convert_kana($fax3, "as", "EUC-JP");
	$fax3 = mb_convert_kana($fax3, "as", "UTF-8");
	$email = $_POST['email'];								//	メール
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);
	$email2 = $_POST['email2'];								//	メール2
#	$email2 = mb_convert_kana($email2, "as", "EUC-JP");
	$email2 = mb_convert_kana($email2, "as", "UTF-8");
	$email2 = trim($email2);
	$haitatu = $_POST['haitatu'];							//	配達時間
	$zaiko = $_POST['zaiko'];								//	在庫なき場合
	$shiharai = $_POST['shiharai'];							//	支払方法
	$point = $_POST['point'];								//	保有ポイント
	$r_point = $_POST['r_point'];							//	ポイント利用フラグ
	$g_point = $_POST['g_point'];							//	利用ポイント数
#	$g_point = mb_convert_kana($g_point, "as", "EUC-JP");
	$g_point = mb_convert_kana($g_point, "as", "UTF-8");
	$msr = $_POST['msr'];									//	ご意見ご要望
#	$msr = mb_convert_kana($msr, "asKV", "EUC-JP");
	$msr = mb_convert_kana($msr, "asKV", "UTF-8");

	//	ユーザー情報エラーチェック
	if ($name_s == "") {
		$ERROR[] = "漢字氏名（姓）が入力されておりません。";
	}
	if ($name_n == "") {
		$ERROR[] = "漢字氏名（名）が入力されておりません。";
	}
	if ($kana_s == "") {
		$ERROR[] = "ふりがな氏名（姓）が入力されておりません。";
	}
	if ($kana_n == "") {
		$ERROR[] = "ふりがな氏名（名）が入力されておりません。";
	}
	if ($zip1 == "" || $zip2 == ""){
		$ERROR[] = "郵便番号が入力されておりません。";
	}
	if ($prf == "") {
		$ERROR[] = "都道府県名が選択されておりません。";
	}
	if ($city == "") {
		$ERROR[] = "市区町村名が入力されておりません。";
	}
	if ($add1 == "") {
		$ERROR[] = "所番地が入力されておりません。";
	}
	if (($tel1 == "" || $tel2 == "" || $tel3 == "") && ($kei1 == "" || $kei2 == "" || $kei3 == "")){
		$ERROR[] = "固定電話または携帯電話どちらかご入力お願いします。 ";
	}
	if ($email == "") {
		$ERROR[] = "E-mailアドレスが入力されておりません。";
	}
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) {
		$ERROR[] = "E-mailのアドレスが不正です。";
	}
	if ($email2 && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email2,$regs)) {
		$ERROR[] = "E-mail2のアドレスが不正です。";
	}
	if (!$zaiko) {
		$ERROR[] = "在庫切れの場合が選択されておりません。";
	}
	if (!$shiharai) {
		$ERROR[] = "支払方法が選択されておりません。";
	}
	if ($r_point == 1 && !$g_point) {
		$ERROR[] = "ご利用割引ポイントが入力されておりません。";
	}

	$point = preg_replace("/[^0-9]/", "", $point);
	$g_point = preg_replace("/[^0-9]/", "", $g_point);
	if ($r_point == 1 && $g_point && $g_point > $point) {
		$ERROR[] = "ご利用割引可能ポイントをこえております。";
	}
	if ($r_point == 2 ) {
		$g_point = 0;
	}

	$_SESSION['addr']['name_s'] = $name_s;
	$_SESSION['addr']['name_n'] = $name_n;
	$_SESSION['addr']['kana_s'] = $kana_s;
	$_SESSION['addr']['kana_n'] = $kana_n;
	$_SESSION['addr']['zip1'] = $zip1;
	$_SESSION['addr']['zip2'] = $zip2;
	$_SESSION['addr']['prf'] = $prf;
	$_SESSION['addr']['city'] = $city;
	$_SESSION['addr']['add1'] = $add1;
	$_SESSION['addr']['add2'] = $add2;
	$_SESSION['addr']['tel1'] = $tel1;
	$_SESSION['addr']['tel2'] = $tel2;
	$_SESSION['addr']['tel3'] = $tel3;
	$_SESSION['addr']['kei1'] = $kei1;
	$_SESSION['addr']['kei2'] = $kei2;
	$_SESSION['addr']['kei3'] = $kei3;
	$_SESSION['addr']['fax1'] = $fax1;
	$_SESSION['addr']['fax2'] = $fax2;
	$_SESSION['addr']['fax3'] = $fax3;
	$_SESSION['addr']['email'] = $email;
	$_SESSION['addr']['email2'] = $email2;
	$_SESSION['addr']['haitatu'] = $haitatu;
	$_SESSION['addr']['zaiko'] = $zaiko;
	$_SESSION['addr']['shiharai'] = $shiharai;
	$_SESSION['addr']['r_point'] = $r_point;
	$_SESSION['addr']['g_point'] = $g_point;
	$_SESSION['addr']['msr'] = $msr;
}



//	お届け先入力フォーム
//function cago_form_html($ERROR , $mode){	//	del ookawara 2014/01/06
function cago_form_html($ERROR){	//	add ookawara 2014/01/06
	global $PRF_N , $HAITATU_N , $ZAIKO_N , $SHIHARAI_N , $point_riyou;

	//	アンドロイド対策
	//	add ookawara 2014/05/13
	$_SESSION['cago_form'] = "back";

	//	PayPalセッション削除
	//	add ookawara	2014/05/13
	unset($_SESSION['paypal_list']);
	// PAYJP データ削除
	unset($_SESSION['payjp']);
	unset($_SESSION['payer_id']);
	unset($_SESSION['curl_error_no']);
	unset($_SESSION['curl_error_msg']);
	unset($_SESSION['nvpReqArray']);
	unset($_SESSION['TOKEN']);
	unset($_SESSION['currencyCodeType']);
	unset($_SESSION['PaymentType']);
	unset($_SESSION['Payment_Amount']);

	$html = "";

	if ($_GET['m'] == "cancel" && $_SESSION['PAYPAL_ERROR']) {
		$ERROR = $_SESSION['PAYPAL_ERROR'];
		unset($_SESSION['PAYPAL_ERROR']);
	}

	if ($_GET['m'] == "cancel" && $_SESSION['PAYJP_ERROR']) {
		$ERROR = $_SESSION['PAYJP_ERROR'];
		unset($_SESSION['PAYJP_ERROR']);
	}

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$idpass = $_SESSION['idpass'];
	$addr = $_SESSION['addr'];

	if ($idpass && !$addr) {
		list($email_,$pass,$check,$af_num) = explode("<>",$idpass);
		$sql =  "SELECT * FROM ".T_KOJIN.
				" WHERE email='".$email_."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000';";
		$sql1 = pg_query(DB,$sql);
		$count = pg_num_rows($sql1);
		if ($count >= 1) {
			list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$n,$n,$point,$saku,$kei1,$kei2,$kei3,$email2) = pg_fetch_array($sql1,0);
			$_SESSION['addr']['point'] = $point;
		}
	} elseif($addr) {
		if ($idpass) {
			list($email_,$pass,$check,$af_num) = explode("<>",$idpass);
			$sql =  "SELECT kojin_num , point FROM ".T_KOJIN.
					" WHERE email='".$email_."'".
					" AND pass='".$pass."'".
					" AND saku!='1'".
					" AND kojin_num<'100000';";
			$sql1 = pg_query(DB,$sql);
			$count = pg_num_rows($sql1);
			if ($count >= 1) {
				list($kojin_num,$point) = pg_fetch_array($sql1,0);
			} else {
				unset($idpass);
				unset($_SESSION['idpass']);
			}
		} else {
			unset($kojin_num);
			unset($idpass);
			unset($_SESSION['idpass']);
		}
	}

	$g_point = 0;	//	add ookawara 2014/01/06
	//if ($mode == "check" || $mode == "modoru") {	//	del ookawara 2014/01/06
	if ($addr) {	//	add ookawara 2014/01/06
		$name_s = $_SESSION['addr']['name_s'];
		$name_n = $_SESSION['addr']['name_n'];
		$kana_s = $_SESSION['addr']['kana_s'];
		$kana_n = $_SESSION['addr']['kana_n'];
		$zip1 = $_SESSION['addr']['zip1'];
		$zip2 = $_SESSION['addr']['zip2'];
		$prf = $_SESSION['addr']['prf'];
		$city = $_SESSION['addr']['city'];
		$add1 = $_SESSION['addr']['add1'];
		$add2 = $_SESSION['addr']['add2'];
		$tel1 = $_SESSION['addr']['tel1'];
		$tel2 = $_SESSION['addr']['tel2'];
		$tel3 = $_SESSION['addr']['tel3'];
		$kei1 = $_SESSION['addr']['kei1'];
		$kei2 = $_SESSION['addr']['kei2'];
		$kei3 = $_SESSION['addr']['kei3'];
		$fax1 = $_SESSION['addr']['fax1'];
		$fax2 = $_SESSION['addr']['fax2'];
		$fax3 = $_SESSION['addr']['fax3'];
		$email = $_SESSION['addr']['email'];
		$email2 = $_SESSION['addr']['email2'];
		$haitatu = $_SESSION['addr']['haitatu'];
		$zaiko = $_SESSION['addr']['zaiko'];
		$shiharai = $_SESSION['addr']['shiharai'];
		$r_point = $_SESSION['addr']['r_point'];
		$g_point = $_SESSION['addr']['g_point'];
		$msr = $_SESSION['addr']['msr'];
	} else {
		//	add ookawara 2014/01/06
		if ($point > 0) {
			$g_point = $point;
		}
	//	unset($_SESSION['addr']);	//	del ookawara 2014/01/06
	}

	if ($email2 == "") {
		unset($email2);
	}

	//	都道府県プルダウン
	$prf_html .= "	<select name=\"prf\" class=\"select-list\">";
	$prf_html .= "		<option value=\"\">選択して下さい。</option>\n";

	for ($i = 1; $i <= 47; $i++) {
		if ($i == $prf) {
			$selected = "selected";
		} else {
			$selected = "";
		}

		$prf_html .= "		<option value=\"".$i."\"".$selected.">".$PRF_N[$i]."</option>\n";

	}
	$prf_html .= "	</select>";

	//	配達ご希望時間プルダウン
	$haitatu_html .= "<select name=\"haitatu\" class=\"select-list\">\n";
	$haitatu_html .= "	<option value=\"0\">特になし</option>\n";
	$max = count($HAITATU_N);
	for($i=1; $i<$max; $i++) {
		if ($haitatu == $i) {
			$selected = "selected=\"selected\"";
		} else {
			$selected = "";
		}
		$haitatu_html .= "	<option value=\"".$i."\"".$selected.">".$HAITATU_N[$i]."</option>\n";
	}
	$haitatu_html .= "</select>\n";

	//	在庫切れの場合プルダウン
	$zaiko_html .= "<select name=\"zaiko\" class=\"select-list\">\n";
	$zaiko_html .= "	<option value=\"0\">選択して下さい。</option>\n";
	$max = count($ZAIKO_N);
	for($i=1; $i<$max; $i++) {
		if (!$ZAIKO_N[$i]) {
			continue;
		}
		if ($zaiko == $i) {
			$selected = "selected=\"selected\"";
		} else {
			$selected = "";
		}
		$zaiko_html .= "	<option value=\"".$i."\"".$selected.">".$ZAIKO_N[$i]."</option>\n";
	}
	$zaiko_html .= "</select>\n";

	//	支払方法プルダウン

	//	PAYPALテスト用
	//if (!$_SESSION['idpass'] && !preg_match("/ookawara@azet.jp/", $_SESSION['idpass']) && !$_SESSION['idpass'] && !preg_match("/polishstar@y8.dion.ne.jp/", $_SESSION['idpass'])) {
	//	unset($SHIHARAI_N[6]);
	//}

	$shiharai_html .= "<select name=\"shiharai\" class=\"select-list\">\n";
	$shiharai_html .= "	<option value=\"0\">選択して下さい。</option>\n";
	foreach ($SHIHARAI_N AS $i => $val) {
		if (pay_limited == 1 && ($i != 1 && $i != 3 && $i != 5)) {
			continue;
		}
		if ($shiharai == $i) {
			$selected = "selected=\"selected\"";
		} else { $selected = "";
		}
		$shiharai_html .= "	<option value=\"".$i."\"".$selected.">".$val."</option>\n";
	}
	$shiharai_html .= "</select>\n";

	//	ポイント利用
	if ($kojin_num && $point && $point_riyou != 1) {
		if (!$r_point || $r_point == 2) {
			$checked1 = "";
			$checked2 = "checked=\"checked\"";
			//$g_point = 0;	//	del ookawara 2014/01/06
		} else {
			$checked1 = "checked=\"checked\"";
			$checked2 = "";
		}
		$point_all = number_format($point);
		$min_point = MIN_POINT;
		if ($min_point > $point) {
			$DEL_INPUTS['POINT'] = 1;			//	ポイントOKhtml削除
		} else {
			$DEL_INPUTS['NOTPOINT'] = 1;		//	ポイントNGhtml削除
		}
	} else {
		$DEL_INPUTS['POINT'] = 1;			//	ポイントOKhtml削除
		$DEL_INPUTS['NOTPOINT'] = 1;		//	ポイントNGhtml削除
	}

	$DEL_INPUTS['TYUUMONHIDE'] = 1;
	$DEL_INPUTS['WARIBIKIDEL'] = 1;			//	会員割引削除
	$DEL_INPUTS['WARIBIKI2DEL'] = 1;		//	非会員割引削除
	$DEL_INPUTS['PAERSENTDEL'] = 1;			//	購入金額割引削除
	$DEL_INPUTS['RPOINTDEL'] = 1;			//	利用ポイント削除
	$DEL_INPUTS['SOURYOUDEL'] = 1;			//	送料あり削除
	$DEL_INPUTS['SOURYOUSDEL'] = 1;			//	送料サービス削除
	$DEL_INPUTS['PAYPALLASTMSG'] = 1;		//	PayPal最終支払い確認時メッセージ		//	add ookawara 2014/03/25
	$DEL_INPUTS['PAYPALLAST'] = 1;			//	PayPal最終支払い確認時ご注文ボタン		//	add ookawara 2014/03/25

	$INPUTS['NAMES'] = $name_s;				//	姓
	$INPUTS['NAMEN'] = $name_n;				//	名
	$INPUTS['KANAS'] = $kana_s;				//	姓：ふりがな
	$INPUTS['KANAN'] = $kana_n;				//	名：ふりがな
	$INPUTS['ZIP1'] = $zip1;				//	郵便番号1
	$INPUTS['ZIP2'] = $zip2;				//	郵便番号2
	$INPUTS['PRF'] = $prf_html;				//	都道府県
	$INPUTS['CITY'] = $city;				//	市区町村名
	$INPUTS['ADD1'] = $add1;				//	所番地
	$INPUTS['ADD2'] = $add2;				//	マンション名など
	$INPUTS['TEL1'] = $tel1;				//	電話番号1
	$INPUTS['TEL2'] = $tel2;				//	電話番号2
	$INPUTS['TEL3'] = $tel3;				//	電話番号3
	$INPUTS['KEI1'] = $kei1;				//	電話番号1
	$INPUTS['KEI2'] = $kei2;				//	電話番号2
	$INPUTS['KEI3'] = $kei3;				//	電話番号3
	$INPUTS['FAX1'] = $fax1;				//	FAX1
	$INPUTS['FAX2'] = $fax2;				//	FAX2
	$INPUTS['FAX3'] = $fax3;				//	FAX3
	$INPUTS['EMAIL'] = $email;				//	メール
	$INPUTS['EMAIL2'] = $email2;			//	メール2
	$INPUTS['HAITATU'] = $haitatu_html;		//	配達ご希望時間帯
	$INPUTS['ZAIKO'] = $zaiko_html;			//	在庫なき場合
	$INPUTS['SHIHARAI'] = $shiharai_html;	//	支払方法
	$INPUTS['CHECKED1'] = $checked1;		//	ポイント使用
	$INPUTS['CHECKED2'] = $checked2;		//	ポイント使用しない
	$INPUTS['POINTALL'] = $point_all;		//	保有ポイント
	$INPUTS['GPOINT'] = $g_point;			//	今回使用するポイント
	$INPUTS['MINPOINT'] = $min_point;		//	最低使用ポイント
	$INPUTS['MSR'] = $msr;					//	ご意見ご要望

	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("cago_form.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	ログインページ
function login_html($ERROR) {

	$html = "";

	if ($ERROR) {
		$error_html = error_html($ERROR);
	}

	$email = $_POST['email'];
	$pass = $_POST['pass'];
	$check = $_POST['check'];

	$checked = "";
	if ($check) {
		$checked = "checked";
	}

	$INPUTS = array();
	$DEL_INPUTS = array();

	$INPUTS['ERROR'] = $error_html;		//	エラーメッセージ
	$INPUTS['EMAIL'] = $email;			//	入力メールアドレス
	$INPUTS['PASS'] = $pass;			//	入力パスワード
	$INPUTS['CHECKED'] = $checked;		//	自動ログインする

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("cago_login.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}

//	ユーザー確認
function checkuser(&$ERROR) {

	$email = $_POST['email'];
#	$email = mb_convert_kana($email, "as", "EUC-JP");
	$email = mb_convert_kana($email, "as", "UTF-8");
	$email = trim($email);
	$pass = $_POST['pass'];
#	$pass = mb_convert_kana($pass, "as", "EUC-JP");
	$pass = mb_convert_kana($pass, "as", "UTF-8");
	$pass = trim($pass);
	$check = $_POST['check'];

	if (!$email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (is_array($ERROR)) {
		return;
	}

	$kojin_num = 0;
	$sql  = "SELECT kojin_num, name_s, point FROM ".T_KOJIN.
			" WHERE email='".$email."'".
			" AND pass='".$pass."'".
			" AND saku!='1'".
			" AND kojin_num<'100000'".
			" ORDER BY kojin_num;";

	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$kojin_num = $list['kojin_num'];
		$name_s = $list['name_s'];
#		$name_s = mb_convert_encoding($name_s, "utf-8", "euc-jp");
		$point = $list['point'];
	}

	if ($kojin_num < 1) {
		$ERROR[] = "登録されてないか入力された情報が間違っています。";
		return;
	}

	//	アフィリエイトユーザーチェック
	$af_num = 0;
	$sql  = "SELECT af_num FROM ".T_AFUSER.
			" WHERE kojin_num='".$kojin_num."'".
			" AND state!='1';";
	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$af_num = $list['af_num'];
		$_SESSION['affid'] = $af_num;	/* add yoshizawa 2013/09/04 */	
	}

	//	情報セッション・クッキセット
	$idpass = $email."<>".$pass."<>".$check."<>".$af_num."<>".$name_s."<>".$point."<>";
	$_SESSION['idpass'] = $idpass;
	unset($_COOKIE['idpass']);
	setcookie("idpass");
	if ($check == 1) {
		setcookie("idpass", $idpass, time() + 60*60*24*30, "/", ".futboljersey.com");
	}

	$url = "/";
	if ($_SESSION['idpass'] && $_SESSION['blurl']) {
		$url = $_SESSION['blurl'];
		unset($_SESSION['blurl']);
	}

}



//	確認ページ
function cago_kakunin_html($KAGOS , $OPTIONS , $ERROR , $mode){

	global   $PRF_N , $HAITATU_N , $ZAIKO_N , $SHIHARAI_N , $TAX_ , $mochi_pri , $PHP_SELF , $SEBAN_P_N , $SENAME_P_N , $MUNEBAN_P_N ,
			 $PANT_P_N , $BACH_P_N , $waribiki , $waribiki2 , $wa_member , $DISCOUNT_C , $DISCOUNT , $DISCOUNT , $SOURYOU_MURYOU , $P_RITU ,
			 $P_RITU , $UN_N , $free_shipping , $DAIBIKI_N , $DAIBIKI_P_N , $CON_TESU , $TESU_P;

	//	paypal送信用セッションクリアする。
	//	add	ookawara	2014/01/24
	if ($mode == "") {
		unset($_SESSION['paypal_list']);
	}

	//	コメントアウト部分の変数
	//$TESU_P = 0;	//	代引き手数料

	$html = "";

	if ($_SESSION['addr']) {

		$name_s = $_SESSION['addr']['name_s'];
		$name_n = $_SESSION['addr']['name_n'];
		$kana_s = $_SESSION['addr']['kana_s'];
		$kana_n = $_SESSION['addr']['kana_n'];
		$zip1 = $_SESSION['addr']['zip1'];
		$zip2 = $_SESSION['addr']['zip2'];
		$prf = $_SESSION['addr']['prf'];
		$city = $_SESSION['addr']['city'];
		$add1 = $_SESSION['addr']['add1'];
		$add2 = $_SESSION['addr']['add2'];
		$tel1 = $_SESSION['addr']['tel1'];
		$tel2 = $_SESSION['addr']['tel2'];
		$tel3 = $_SESSION['addr']['tel3'];
		$kei1 = $_SESSION['addr']['kei1'];
		$kei2 = $_SESSION['addr']['kei2'];
		$kei3 = $_SESSION['addr']['kei3'];
		$fax1 = $_SESSION['addr']['fax1'];
		$fax2 = $_SESSION['addr']['fax2'];
		$fax3 = $_SESSION['addr']['fax3'];
		$pass = $_SESSION['addr']['pass'];
		$email = $_SESSION['addr']['email'];
		$email2 = $_SESSION['addr']['email2'];
		$haitatu = $_SESSION['addr']['haitatu'];
		$zaiko = $_SESSION['addr']['zaiko'];
		$shiharai = $_SESSION['addr']['shiharai'];
		$r_point = $_SESSION['addr']['r_point'];
		$g_point = $_SESSION['addr']['g_point'];
		$msr = $_SESSION['addr']['msr'];
		$msr = nl2br($msr);

	}

	//	商品表示
	unset($customer);
	$price_all = 0;
	$souryou_muryou_flag = 0;
	if ($KAGOS) {

		// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"syouhin\">\n";
		// $html .= "			商品番号\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"tanka\" rowspan=\"2\">\n";
		// $html .= "			単価\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"suuryou\" rowspan=\"2\">\n";
		// $html .= "			数量\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"syoukei\" rowspan=\"2\">\n";
		// $html .= "			小計\n";
		// $html .= "		</th>\n";
		// $html .= "	</tr>\n";
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"sakuzyo\">商品名</th>\n";
		// $html .= "	</tr>\n";

		$html .= "<div class=\"marking-title-section\">\n";
		$html .= "	<div class=\"marking-title\">\n";
		$html .= "		<span class=\"item-name\">商品名</span>\n";
		$html .= "		<span>単価</span>\n";
		$html .= "  	<span>数量</span>\n";
		$html .= "  	<span>小計</span>\n";
		$html .= "	</div>\n";
		$html .= "</div>\n";

		$pay_option_num = 1;	//	add ookawara 2014/01/24
		foreach ($KAGOS AS $val) {
			$syoukei = 0;
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
			if ($hinban_ == "") {
				continue;
			} else {
				$customer .= $val."<>";
			}
			$_SESSION['customer'] = $customer;

			//	送料無料チェック
			$hinban_id = "";
			$GDNM = explode("-",$hinban_);
			$gdnm_max = count($GDNM) - 1;
			for ($i=0; $i<$gdnm_max; $i++) {
				if ($hinban_id) {
					$hinban_id .= "-";
				}
				$hinban_id .= $GDNM[$i];
			}

			if ($SOURYOU_MURYOU[$hinban_id]) {
				$souryou_muryou_flag = 1;
			}

			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $num_;
			$price_all = $price_all + $syoukei;
			$kakaku_h = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
			// $html .= "	<tr>\n";
			// $html .= "		<td class=\"cago_title\">\n";
			// $html .= "			".$hinban_."\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">\n";
			// $html .= "			".$kakaku_h." 円\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">\n";
			// $html .= "			".$num_."\n";
			// $html .= "		</td>\n";
			// $html .= "		<td rowspan=\"2\">".$syoukei." 円</td>\n";
			// $html .= "	</tr>\n";
			// $html .= "	<tr>\n";
			// $html .= "		<td class=\"cago_title\">".$title_."</td>\n";
			// $html .= "	</tr>\n";

			$html .= "<section class=\"goods-total\">\n";
			$html .= "	<div class=\"goods-title\">\n";
			$html .= "		<div class=\"item-title confirmation-form\">\n";
			$html .= "			<span class=\"bold\">".$title_."</span>\n";
			$html .= "		</div>\n";
			$html .= "	</div>\n";
			$html .= "	<div class=\"goods-detail\">\n";
			$html .= "		<div class=\"item-name\">\n";
			$html .= "			<span>商品番号：".$hinban_."</span>\n";
			$html .= "		</div>\n";
			$html .= "		<div>\n";
			$html .= "			<span>".$kakaku_h." 円</span>\n";
			$html .= "		</div>\n";
			$html .= "		<div>\n";
			$html .= "			<span>x".$num_."</span>\n";
			$html .= "		</div>\n";
			$html .= "		<div>\n";
			$html .= "			<span class=\"bold\">".$syoukei." 円</span>\n";
			$html .= "		</div>\n";
			$html .= "	</div>\n";
			$html .= "</section>\n";

			//	paypal用データ作成
			//	add ookawara 2014/01/24
			set_paypal_list($title_, $hinban_, $num_, $kakaku_h);

		}
	}

	//	マーキングオーダー表示
	if (!$KAGOS && $OPTIONS) {

		// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $html .= "	<tr>\n";
		// $html .= "		<th class=\"syouhin\">\n";
		// $html .= "			商品名\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"tanka\">\n";
		// $html .= "			単価\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"suuryou\">\n";
		// $html .= "			数量\n";
		// $html .= "		</th>\n";
		// $html .= "		<th class=\"syoukei\">\n";
		// $html .= "			小計\n";
		// $html .= "		</th>\n";
		// $html .= "	</tr>\n";

		$html .= "<div class=\"marking-title-section\">\n";
		$html .= "	<div class=\"marking-title\">\n";
		$html .= "		<span class=\"item-name\">商品名</span>\n";
		$html .= "		<span>単価</span>\n";
		$html .= "  	<span>数量</span>\n";
		$html .= "  	<span>小計</span>\n";
		$html .= "	</div>\n";
		$html .= "</div>\n";
	}

	if ($OPTIONS) {

		$pay_option_price = 0;	//	add ookawara 2014/01/24
		unset($opt);
		foreach ($OPTIONS AS $val) {

			$syoukei = 0;
			list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);

			if ($hinban_ == "") {
				continue;
			} else {
				$opt .= $val."<>";
			}
			$_SESSION['opt'] = $opt;	//	削除後の商品データ（配列）をもとに$_SESSION['opt']を書き換える

			// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
			// $marking_html .= "<tr>\n";
			// $marking_html .= "	<td class=\"cago_title cago_mar\" colspan=\"4\">\n";
			// $marking_html .= "		マーキング 商品名：".$title_;
			// $marking_html .= "	</td>\n";
			// $marking_html .= "</tr>\n";

			$marking_html .= "<section class=\"marking-total\">\n";
			$marking_html .= "	<div class=\"marking-total-detail\">\n";
			$marking_html .= "		<div class=\"marking-item-name\">\n";
			$marking_html .= "			<div class=\"item-title confirmation-form\">\n";
			$marking_html .= "				<span class=\"bold\">マーキング 商品名：".$title_."</span>\n";
			$marking_html .= "			</div>\n";
			$marking_html .= "		</div>\n";
			$marking_html .= "	</div>\n";

			//	持ち込み手数料
			if ($hinban_ == "mochikomi") {
				$kakaku_ = $mochi_pri;
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_;
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">持ち込み手数料</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>1</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div class=\"item-name\">\n";
				$marking_html .= "		<span>持ち込み手数料</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x1</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei."円<span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
			}

			//	背番号
			if ($seban_l_) {
				$moji_num = strlen($seban_num_);				//文字列のながさ
				$kakaku_ = $SEBAN_P_N[$seban_l_];				//タイプの価格
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);	//端数の切り捨て
				$syoukei = $kakaku_ * $moji_num;				//番号の数×値段
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;				//合計金額＋$syoukei
				$kakaku_ = number_format($kakaku_);				//数字を千位毎にグループ化してフォーマットする
				$syoukei = number_format($syoukei);				//数字を千位毎にグループ化してフォーマットする

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">背番号 ".$SEBAN_N[$seban_l_]." 番号：".$seban_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div class=\"item-name\">\n";
				$marking_html .= "		<span>背番号".$SEBAN_N[$seban_l_]."：".$seban_num_."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x".$moji_num."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
			}

			//	背ネーム
			if ($sename_l_) {
				$sename_name_ = str_replace('\\', '', $sename_name_);
				$sename_name_m = str_replace(' ', '', $sename_name_);
				$moji_num = strlen($sename_name_m);
				$kakaku_ = $SENAME_P_N[$sename_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">背ネーム ".$SENAME_N[$sename_l_]." ネーム：".$sename_name_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div  class=\"item-name\">\n";
				$marking_html .= "		<span>背ネーム ".$SENAME_N[$sename_l_]."：".$sename_name_."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x".$moji_num."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
			}

			//	胸番号
			if ($muneban_l_) {
				$moji_num = strlen($muneban_num_);
				$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">胸番号 ".$MUNEBAN_N[$muneban_l_]." 番号：".$muneban_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div  class=\"item-name\">\n";
				$marking_html .= "		<span>胸番号 ".$MUNEBAN_N[$muneban_l_]."：".$muneban_num_."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x".$moji_num."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
			}

			//	パンツ番号
			if ($pant_l_) {
				$moji_num = strlen($pant_num_);
				$kakaku_ = $PANT_P_N[$pant_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $moji_num;
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">パンツ番号 ".$PANT_N[$pant_l_]." 番号：".$pant_num_."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>".$moji_num."</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div  class=\"item-name\">\n";
				$marking_html .= "		<span>パンツ番号 ".$PANT_N[$pant_l_]."：".$pant_num_."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x".$moji_num."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
			}

			//	バッジ
			if ($bach_l_) {
				$kakaku_ = $BACH_P_N[$bach_l_];
				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_;
				$pay_option_price += $syoukei;					//	add ookawara 2014/01/24
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
				// $marking_html .= "<tr>\n";
				// $marking_html .= "	<td class=\"cago_title cago_mar\">バッジ ".$BACH_N[$bach_l_]."</td>\n";
				// $marking_html .= "	<td>".$kakaku_." 円</td>\n";
				// $marking_html .= "	<td>1</td>\n";
				// $marking_html .= "	<td>".$syoukei." 円</td>\n";
				// $marking_html .= "</tr>\n";

				$marking_html .= "<div class=\"marking-details\">\n";
				$marking_html .= "	<div  class=\"item-name\">\n";
				$marking_html .= "		<span>バッジ ".$BACH_N[$bach_l_]."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>".$kakaku_." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span>x".$moji_num."</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "	<div>\n";
				$marking_html .= "		<span class=\"bold\">".$syoukei." 円</span>\n";
				$marking_html .= "	</div>\n";
				$marking_html .= "</div>\n";
				$marking_html .= "</section>\n";
			}
		}

		//	paypal用データ作成
		//	add ookawara 2014/01/24
		set_paypal_list('マーキング'.$pay_option_num, '', '', $pay_option_price);

		$pay_option_num += 1;	//	add ookawara 2014/01/24
	}

	$idpass = $_SESSION['idpass'];
	list($id,$pass) = explode("<>",$idpass);
	$sql =  "SELECT kojin_num FROM ".T_KOJIN.
			" WHERE email='".$id."'".
			" AND pass='".$pass."'".
			" AND saku!='1'".
			" AND kojin_num<'100000';";
	$sql1 = pg_query(DB,$sql);
	$count = pg_numrows($sql1);
	if ($count >= 1) {
		list($kojin_num) = pg_fetch_array($sql1,0);
	}

//$waribiki = 10;
//$waribiki2 = 30;
//$DISCOUNT_C = 1;
	//	割引表示
	if ($idpass && $waribiki > 0) {

		list($id,$pass) = explode("<>",$idpass);
		$sql =  "SELECT kojin_num FROM ".T_KOJIN.
				" WHERE email='".$id."'".
				" AND pass='".$pass."'".
				" AND saku!='1'".
				" AND kojin_num<'100000';";
		$sql1 = pg_query(DB,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 1) {
			list($kojin_num) = pg_fetch_array($sql1,0);
		}
		if ($kojin_num <= $wa_member) {
			$nebiki = $price_all * $waribiki / 100;
			$price_all = $price_all - $nebiki;
			$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
			$nebiki = number_format($nebiki);

			//$DEL_INPUTS['WARIBIKIDEL'] = 1;	//	会員割引削除
			$DEL_INPUTS['WARIBIKI2DEL'] = 1;	//	非会員割引削除
			$DEL_INPUTS['PAERSENTDEL'] = 1;		//	購入金額割引削除

		}

	} elseif (!$idpass && $waribiki2 > 0) {

		$nebiki = $price_all * $waribiki2 / 100;
		$price_all = $price_all - $nebiki;
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		$DEL_INPUTS['WARIBIKIDEL'] = 1;		//	会員割引削除
		//$DEL_INPUTS['WARIBIKI2DEL'] = 1;	//	非会員割引削除
		$DEL_INPUTS['PAERSENTDEL'] = 1;		//	購入金額割引削除

	} elseif ($DISCOUNT_C == 1) {

		unset($nebiki);
		$DISCOUNT = array_reverse($DISCOUNT);
		foreach ($DISCOUNT AS $VAL) {
			$totalprice_ = $VAL[0];
			$paersent_ = $VAL[1];
			if ($price_all > $totalprice_) {
				$paersent = $paersent_;
				break;
			}
		}
		$nebiki = $price_all * $paersent / 100;
		$price_all = $price_all - $nebiki;
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		if ($nebiki) {
			$DEL_INPUTS['WARIBIKIDEL'] = 1;		//	会員割引削除
			$DEL_INPUTS['WARIBIKI2DEL'] = 1;	//	非会員割引削除
			//$DEL_INPUTS['PAERSENTDEL'] = 1;	//	購入金額割引削除
		}

	} else {
		$DEL_INPUTS['WARIBIKIDEL'] = 1;		//	会員割引削除
		$DEL_INPUTS['WARIBIKI2DEL'] = 1;	//	非会員割引削除
		$DEL_INPUTS['PAERSENTDEL'] = 1;		//	購入金額割引削除
	}

	$INPUTS['WARIBIKI'] = $waribiki;		//	会員割引率
	$INPUTS['WARIBIKI2'] = $waribiki2;		//	非会員割引率
	$INPUTS['PAERSENT'] = $paersent;		//	購入金額割引率
	$INPUTS['NEBIKI'] = $nebiki;			//	値引き額

	//	paypal用データ作成（値引きがあった場合）
	//	add ookawara 2014/01/24
	if ($nebiki) {
		$pay_nebiki = 0 - $nebiki;
		set_paypal_list('割引', '', '', $pay_nebiki);
	}

	//	今回の買い物で使用するポイントとそれを引いた代金
	if ($r_point == 1 && $g_point) {
		$p_all = $price_all;
		$g_point_p = $g_point;
		$p_all = $p_all - $g_point_p;
		if ($p_all <= 0) {
			$g_point_p = $g_point_p + $p_all;
			$g_point = ceil($g_point_p);
			$price_all = 0;
		}else {
			$price_all = $p_all;
		}

		//	paypal用データ作成（値引きがあった場合）
		//	add ookawara 2014/01/24
		$pay_nebiki = 0 - $g_point;
		set_paypal_list('割引ポイント利用', '', '', $g_point_p);

	}

	//	今回の買い物で得るポイント
	$k_point = $price_all * ($P_RITU / 100);
	$k_point = floor($k_point + 0.5);

	//	都道府県別の運賃計算（2013/08/30現在　全国一律665円）
	$unchin = $UN_N[$prf];
	$unchin = floor($unchin * ($TAX_ + 1) + 0.5);

	//	送料無料チェック
	$sevice_check = 0;
	if ($free_shipping != "" && $free_shipping <= $price_all) {
		unset($unchin);
		$sevice_check = 1;
	} elseif ($souryou_muryou_flag == 1) {
		unset($unchin);
	}

	//	paypal用データ作成（送料）
	//	add ookawara 2014/01/24
	$pay_unchin = 0;
	if ($unchin > 0) {
		$pay_unchin = $unchin;
	}
	set_paypal_list('送料', '', '', $pay_unchin);


	//	支払方法
	$pay_siharai_name = "";	//	add ookawara 2014/01/24
	$pay_siharai_price = 0;	//	add ookawara 2014/01/24
	//	代金引換
	if ($shiharai == 1) {
//		$TESU_P = floor($TESU_P * ($TAX_ + 1) + 0.5);
//		$price_all = $price_all + $TESU_P + $unchin;
		$price_all = $price_all + $unchin;
		$max = count($DAIBIKI_N);
		$daibiki = "";
		for($i=0; $i<$max; $i++) {
			$p_all = $price_all;
			//	支払金額が０の場合
			if ($p_all < 1) {
				$daibiki = "";
				$tax = "";
				$price_all = "";
				break;
			}

			if (!$daibiki && $DAIBIKI_N[$i] >= $p_all) {
				$daibiki = $DAIBIKI_P_N[$i];
				$daibiki = floor($daibiki * ($TAX_ + 1) + 0.5);
				$p_all = $p_all + $daibiki;
//				$tax = $p_all * $TAX_ + 0.5;
//				$tax = floor($tax);
//				$p_all = $p_all + $tax;
				if ($DAIBIKI_N[$i] >= $p_all) {
					$price_all = $p_all;
				} else {
					$daibiki = "";
					$tax = "";
				}
			}
		}

		$pay_siharai_name = "代引手数料";	//	add ookawara 2014/01/24
		$pay_siharai_price = $daibiki;	//	add ookawara 2014/01/24

	//	(前払い)コンビニ払い
	} elseif ($shiharai == 4) {
		$con_tesu = floor($CON_TESU * ($TAX_ + 1) + 0.5);
		$price_all = $price_all + $unchin + $con_tesu;

		$pay_siharai_name = "請求書発行手数料";	//	add ookawara 2014/01/24
		$pay_siharai_price = $con_tesu;	//	add ookawara 2014/01/24

	//	後払い手数料計算	add ookawara 2010/12/10
	} elseif ($shiharai == 5) {
		$ato_tesu = floor(($price_all+ $unchin) * (atobarai / 100) + 0.5);
		if ($ato_tesu < ato_low_price) {
			$ato_tesu = ato_low_price;
		}
		$price_all = $price_all + $unchin + $ato_tesu;

		$pay_siharai_name = "後払い決算手数料";	//	add ookawara 2014/01/24
		$pay_siharai_price = $ato_tesu;	//	add ookawara 2014/01/24

	//	代金引換、(前払い)コンビニ払い、後払い　以外
	} else {
		$price_all = $price_all + $unchin;
//		$tax = $price_all * $TAX_ + 0.5;
//		$tax = floor($tax);
//		$price_all = $price_all + $tax;
	}

	$g_point_p = number_format($g_point_p);	//	利用ポイント
	$k_point = number_format($k_point);		//	獲得ポイント
//	$tax = number_format($tax);
	$daibiki = number_format($daibiki);		//	代引き手数料
	$tesu = number_format($tesu);			//	？？？
	$unchin = number_format($unchin);		//	運賃
	$price_all_ = $price_all;				//	合計金額（割引済み）
	$price_all = number_format($price_all);

	//	利用ポイント表示
	if ($r_point == 1 && $g_point) {
		$INPUTS['RPOINTN'] = $g_point_p;		//	利用ポイント
	} else {
		$DEL_INPUTS['RPOINTDEL'] = 1;			//	利用ポイント削除
	}

	//	送料表示
	if ($sevice_check == 1 || $souryou_muryou_flag == 1) {
		$DEL_INPUTS['SOURYOUDEL'] = 1;			//	送料あり削除
	} else {
		$INPUTS['UNTINN'] = $unchin;			//	運賃出力
		$DEL_INPUTS['SOURYOUSDEL'] = 1;			//	送料サービス削除
	}

	//	支払方法
	//	代引き
	if ($shiharai == 1) {
		//2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $goukei_html .= "<tr>\n";
		// $goukei_html .= "	<th class=\"pay_information\" colspan=\"3\">代引手数料</th>\n";
		// $goukei_html .= "	<th>".$daibiki." 円</th>\n";
		// $goukei_html .= "</tr>\n";

		$goukei_html .= "<div class=\"postage-detail bold\">\n";
		$goukei_html .= "	<span class=\"first-item\">代引手数料</span>\n";
		$goukei_html .= "	<span>".$daibiki." 円</span>\n";
		$goukei_html .= "</div>\n";

	//	(前払い)コンビニ払い	/*2013/09/02 yoshizawa プルダウンにコンビニ払いなし*/
	} elseif ($shiharai == 4) {
		//2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $goukei_html .= "<tr>\n";
		// $goukei_html .= "	<th class=\"pay_information\" colspan=\"3\" >請求書発行手数料</th>\n";
		// $goukei_html .= "	<th>".$con_tesu." 円</th>\n";
		// $goukei_html .= "</tr>\n";

		$goukei_html .= "<div class=\"postage-detail bold\">\n";
		$goukei_html .= "	<span class=\"first-item\">請求書発行手数料</span>\n";
		$goukei_html .= "	<span>".$con_tesu." 円</span>\n";
		$goukei_html .= "</div>\n";

	//	後払い手数料追加 2010/12/10 add ookawara
	} elseif ($shiharai == 5) {
		$atobarai = atobarai;
		//2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $goukei_html .= "<tr>\n";
		// $goukei_html .= "	<th class=\"pay_information\" colspan=\"3\">後払い決算手数料(全代金".$atobarai."% 但し250円未満の場合は、250円。)</th>\n";
		// $goukei_html .= "	<th >".$ato_tesu." 円</th>\n";
		// $goukei_html .= "</tr>\n";

		$goukei_html .= "<div class=\"postage-detail bold\">\n";
		$goukei_html .= "	<span class=\"first-item\">後払い決算手数料(全代金".$atobarai."% 但し250円未満の場合は、250円。)</span>\n";
		$goukei_html .= "	<span>".$ato_tesu." 円</span>\n";
		$goukei_html .= "</div>\n";
	}


	$del_not_paypal = 0; //注文ボタン状態
	//	支払い方法がPAY.JPだった場合の処理
	$hide_payjp=1;// PAY.JPスクリプト非表示
	if ($shiharai==7){
		$del_not_paypal=1; //注文ボタン非表示
		$hide_payjp=0;// PAY.JPスクリプト非表示
		$_SESSION['payjp']['payment_amount']=$price_all_; //支払金額
		$ui_param=getUIparams();
		$INPUTS["PAYJPKEY"]=$ui_param["data_key"];
		$INPUTS["PAYJPLANG"]=$ui_param["language"];
		$INPUTS["PAYJPPLACEHOLDER"]=$ui_param["name_placeholder"];
		$INPUTS["PAYJPBUTTON"]=$ui_param["button_text"];
		$INPUTS["PAYJPSUBMIT"]=$ui_param["submit_text"];
		$INPUTS["PAYJPSRC"]=$ui_param["script_source"];
	}

	//	支払い方法がPayPalだった場合の処理
	//	add ookawara 2013/11/15
	$del_paypal = 1;
	unset($_SESSION['paypal']);
	if ($shiharai == 6) {
		$del_not_paypal = 1;
		$del_paypal = 0;
	}
	$_SESSION['paypal_list']['TOTAL'] = $price_all_;	//	add ookawara 2013/11/19

/*	del yoshizawa 2014/01/06 未使用なのでコメントアウトする
	//	合計金額（商品ありページ）
	if($mode == "" || $mode == "hen"|| $mode == "del"|| $mode == "del_op"){
		if ($souryou_muryou_flag == 1) {
			$goukei_html .= "<tr>\n";
			$goukei_html .= "	<th colspan=\"3\">送料</th>\n";
			$goukei_html .= "	<th><span class=\"red\">サービス</span></th>\n";
			$goukei_html .= "</tr>\n";
			$goukei_html .= "<tr>\n";
			$goukei_html .= "	<th colspan=\"3\">手数料</th>\n";
			$goukei_html .= "	<th>未定</th>\n";
			$goukei_html .= "</tr>\n";
			$goukei_html .= "<tr>\n";
			$goukei_html .= "	<th colspan=\"3\">合計金額</th>\n";
			$goukei_html .= "	<th colspan=\"2\">".$price_all." 円 + 手数料</th>\n";
			$goukei_html .= "</tr>\n";
		} else {
			$goukei_html .= "<tr>\n";
			$goukei_html .= "	<th colspan=\"3\">送料・手数料</th>\n";
			$goukei_html .= "	<th>未定</th>\n";
			$goukei_html .= "</tr>\n";
			$goukei_html .= "<tr>\n";
			$goukei_html .= "	<th colspan=\"3\">合計金額</th>\n";
			$goukei_html .= "	<th colspan=\"2\">".$price_all." 円 + 送料・手数料</th>\n";
			$goukei_html .= "</tr>\n";
		}
	}
*/
	//	合計金額
	// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
	// $goukei_html .= "<tr>\n";
	// $goukei_html .= "	<th class=\"pay_information\" colspan=\"3\">合計金額</th>\n";
	// $goukei_html .= "	<th>".$price_all." 円 </th>\n";
	// $goukei_html .= "</tr>\n";

	$goukei_html .= "<div class=\"postage-detail bold\">\n";
	$goukei_html .= "	<span class=\"first-item\">合計金額</span>\n";
	$goukei_html .= "	<span>".$price_all." 円</span>\n";
	$goukei_html .= "</div>\n";

	//	獲得ポイント表示
	if ($kojin_num && $idpass && $k_point) {
		// 2022/12/7 レスポンシブ対応につき、コメントアウト uenishi
		// $goukei_html .= "<tr>\n";
		// $goukei_html .= "	<th class=\"pay_information\" colspan=\"3\">獲得予定割引ポイント</th>\n";
		// $goukei_html .= "	<th>".$k_point."pt</th>\n";
		// $goukei_html .= "</tr>\n";

		$goukei_html .= "<div class=\"postage-detail bold\">\n";
		$goukei_html .= "	<span class=\"first-item\">獲得予定割引ポイント</span>\n";
		$goukei_html .= "	<span>".$k_point."pt</span>\n";
		$goukei_html .= "</div>\n";
	}

	//	ページヘッドタイトル
	//	add ookawara 2014/03/25
	$page_head_title = "お届け先情報フォーム";
	if ($mode == "check") {
		$page_head_title = "ご注文内容確認";
	} elseif ($mode == "paypal_last_check") {
		$page_head_title = "PayPal最終支払い確認";
	}
	$INPUTS['PAGEHEADTITLE'] = $page_head_title;

	if ($mode != "paypal_last_check") {
//		$DEL_INPUTS['PAGEHEADTITLE'] = 1;	//	PayPal最終支払い確認
	}

	//	PayPal最終支払い確認時お客様情報削除
	$paypallastdel = 0;
	$paypallast = 0;
	if ($mode == "paypal_last_check") {
		$paypallastdel = 1;
		$del_paypal = 1;
	} else {
		$paypallast = 1;
	}

	$DEL_INPUTS['TYUUIDEL'] = 1;	//	注意文削除

	if($zip1 && $zip2){
		$zipn = "〒".$zip1."-".$zip2;
	}
	$INPUTS['ZIPN'] = $zipn;		//	郵便番号表示

	if($tel1 && $tel2 && $tel3){
		$teln = "固定電話：".$tel1."-".$tel2."-".$tel3;
	}
	$INPUTS['TELN'] = $teln;		//	電話番号表示

	if($kei1 && $kei2 && $kei3){
		$kein = "携帯電話：".$kei1."-".$kei2."-".$kei3;
	}
	$INPUTS['KEIN'] = $kein;		//	携帯番号表示

	if($fax1 && $fax2 && $fax3){
		$faxn = $fax1."-".$fax2."-".$fax3;
	}
	$INPUTS['FAXN'] = $faxn;		//	FAX番号表示

	if($email){
		$emailn = "E-mail1：".$email;
	}
	$INPUTS['MAIL1N'] = $emailn;	//	メールアドレス表示

	if($email2){
		$email2n = "E-mail2：".$email2;
	}
	$INPUTS['MAIL2N'] = $email2n;	//	メールアドレス2表示

	//	入力フォーム部分削除
	$DEL_INPUTS['NAMEDEL'] = 1;
	$DEL_INPUTS['KANADEL'] = 1;
	$DEL_INPUTS['ZIPDEL'] = 1;
	$DEL_INPUTS['CITYDEL'] = 1;
	$DEL_INPUTS['ADD1DEL'] = 1;
	$DEL_INPUTS['ADD2DEL'] = 1;
	$DEL_INPUTS['TELDEL'] = 1;
	$DEL_INPUTS['KEIDEL'] = 1;
	$DEL_INPUTS['FAXDEL'] = 1;
	$DEL_INPUTS['EMAIL1DEL'] = 1;
	$DEL_INPUTS['EMAIL2DEL'] = 1;
	$DEL_INPUTS['TEXTDEL'] = 1;
	$DEL_INPUTS['MSRDEL'] = 1;
	$DEL_INPUTS['KAKUNINDEL'] = 1;
	$DEL_INPUTS['POINT'] = 1;						//	ポイントOKhtml削除
	$DEL_INPUTS['NOTPOINT'] = 1;					//	ポイントNGhtml削除
	$DEL_INPUTS['NOTPAYPAL'] = $del_not_paypal;		//	注文ボタン			//	add ookawara 2013/11/15
	$DEL_INPUTS['PAYJP']=$hide_payjp;   			// PAYJP ボタン
	$DEL_INPUTS['PAYPAL'] = $del_paypal;			//	PayPal注文ボタン	//	add ookawara 2013/11/15
	$DEL_INPUTS['PAYPALLASTMSG'] = $paypallastmsg;	//	PayPal最終支払い確認時メッセージ		//	add ookawara 2014/03/25
	$DEL_INPUTS['PAYPALLASTDEL'] = $paypallastdel;	//	PayPal最終支払い確認時お客様情報削除	//	add ookawara 2014/03/25
	$DEL_INPUTS['PAYPALLAST'] = $paypallast;		//	PayPal最終支払い確認時ご注文ボタン		//	add ookawara 2014/03/25

	$INPUTS['GOODS'] = $html;						//	商品表示
	$INPUTS['MARKING'] = $marking_html;				//	マーキング商品表示
	$INPUTS['GOUKEI'] = $goukei_html;				//	合計金額

	$INPUTS['NAMESN'] = $name_s;					//	姓
	$INPUTS['NAMENN'] = $name_n;					//	名
	$INPUTS['KANASN'] = $kana_s;					//	姓：ふりがな
	$INPUTS['KANANN'] = $kana_n;					//	名：ふりがな
	$INPUTS['PRFN'] = $PRF_N[$prf];					//	都道府県
	$INPUTS['CITYN'] = $city;						//	市区町村名
	$INPUTS['ADD1N'] = $add1;						//	所番地
	$INPUTS['ADD2N'] = $add2;						//	マンション名など
	$INPUTS['HAITATUN'] = $HAITATU_N[$haitatu];		//	配達ご希望時間
	$INPUTS['ZAIKON'] = $ZAIKO_N[$zaiko];			//	在庫なき場合
	$INPUTS['SHIHARAIN'] = $SHIHARAI_N[$shiharai];	//	支払方法
	$INPUTS['POINT'] = $point_html;					//	ポイント
	$INPUTS['MSRN'] = $msr;							//	ご意見ご要望

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("cago_form.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}



//	paypal用データ作成
//	add ookawara 2014/01/24
function set_paypal_list($title, $hinban, $num, $kakaku_h) {

	$cnt = COUNT($_SESSION['paypal_list']);
	//	商品名
	$pay_goods_name = "L_PAYMENTREQUEST_0_NAME".$cnt;
	$_SESSION['paypal_list'][$cnt][$pay_goods_name] = $title;
	//	商品番号
	$pay_goods_hinban = "L_PAYMENTREQUEST_0_NUMBER".$cnt;
	$_SESSION['paypal_list'][$cnt][$pay_goods_hinban] = $hinban;
	//	購入数
	$pay_goods_num = "L_PAYMENTREQUEST_0_QTY".$cnt;
	$_SESSION['paypal_list'][$cnt][$pay_goods_num] = $num;
	//	商品表示価格
	$pay_goods_kakaku = "L_PAYMENTREQUEST_0_AMT".$cnt;
	$_SESSION['paypal_list'][$cnt][$pay_goods_kakaku] = $kakaku_h;
	//	合計金額
	//$_SESSION['paypal_list']['TOTAL'] += $kakaku_h * $num;

}



	//	注文＆メール送信
function cago_sent($KAGOS , $OPTIONS , $mode){

	global $wa_member , $waribiki , $waribiki2 , $DISCOUNT_C , $DISCOUNT , $SOURYOU_MURYOU , $mochi_pri , $TAX_ , $SEBAN_N , $SEBAN_P_N ,
		   $SENAME_N , $SENAME_P_N , $MUNEBAN_N , $MUNEBAN_P_N , $PANT_N , $PANT_P_N , $BACH_P_N , $P_RITU , $UN_N , $free_shipping ,
		   $DAIBIKI_N , $DAIBIKI_P_N , $TESU_P , $CON_TESU , $HAITATU_N , $ZAIKO_N , $SHIHARAI_N , $m_footer , $aff_ritsu , $admin_name ,
		   $admin_mail, $PRF_N;

	$customer = $_SESSION["customer"];
	$opt = $_SESSION["opt"];
	$addr = $_SESSION['addr'];

	//	不正画面
	if (!$customer && !$opt && !$addr) {
		header ("Location: ../fusei.htm\n\n");
		exit();
	}

	//	買い物情報
	if ($mode == "send") {
		$name_s = $_SESSION['addr']['name_s'];
		$name_n = $_SESSION['addr']['name_n'];
		$kana_s = $_SESSION['addr']['kana_s'];
		$kana_n = $_SESSION['addr']['kana_n'];
		$zip1 = $_SESSION['addr']['zip1'];
		$zip2 = $_SESSION['addr']['zip2'];
		$prf = $_SESSION['addr']['prf'];
		$city = $_SESSION['addr']['city'];
		$add1 = $_SESSION['addr']['add1'];
		$add2 = $_SESSION['addr']['add2'];
		$tel1 = $_SESSION['addr']['tel1'];
		$tel2 = $_SESSION['addr']['tel2'];
		$tel3 = $_SESSION['addr']['tel3'];
		$kei1 = $_SESSION['addr']['kei1'];
		$kei2 = $_SESSION['addr']['kei2'];
		$kei3 = $_SESSION['addr']['kei3'];
		$fax1 = $_SESSION['addr']['fax1'];
		$fax2 = $_SESSION['addr']['fax2'];
		$fax3 = $_SESSION['addr']['fax3'];
		$email = $_SESSION['addr']['email'];
		$email2 = $_SESSION['addr']['email2'];
		$haitatu = $_SESSION['addr']['haitatu'];
		$zaiko = $_SESSION['addr']['zaiko'];
		$shiharai = $_SESSION['addr']['shiharai'];
		$r_point = $_SESSION['addr']['r_point'];
		$g_point = $_SESSION['addr']['g_point'];
		$msr = $_SESSION['addr']['msr'];
	} else {
		unset($_SESSION['addr']);
	}

	// PAYJP支払い時、最終支払いセット
	$charge_id=null;
	if ($shiharai==7){
		payjpCharge();
		$charge_id=$_SESSION['payjp']["charge_id"];
	}

	//	PayPal支払い時、最終支払いセット
	//	add ookawara 2014/04/03
	if ($shiharai == 6) {
		last_set_paypal();
	}

	//	a8用商品番号
	$a8_num = 1;

	//	$kojin_numを取得
	$idpass = $_SESSION['idpass'];
	if ($idpass != "" && $kojin_num == "") {
		list($id,$pass,$check) = explode("<>",$idpass);
		$sql =  "SELECT kojin_num FROM ".T_KOJIN.
				" WHERE email='".$id."'".
				" AND saku='0';";
		$sql1 = pg_query(DB,$sql);
		$check = pg_num_rows($sql1);
		if ($check <= 0) {
			$idpass = "";
		} else {
			list($kojin_num) = pg_fetch_array($sql1,0);
		}
	}

	//	商品番号決め（時間）
	$sells_num = date("ymdHis");

	//	A8用注文番号
	$_SESSION['a8']['so'] = "ORG-".$sells_num;

	//	非会員処理
	if (!$idpass) {
		$sql =  "SELECT MAX(kojin_num) FROM ".T_KOJIN.
				" WHERE kojin_num>='100001';";
		$sql1 = pg_query(DB,$sql);
		list($kojin_num) = pg_fetch_array($sql1,0);
		if ($kojin_num) {
			$kojin_num = $kojin_num + 1;
		} else {
			$kojin_num = 100001;
		}

		$sql =  "INSERT INTO ".T_KOJIN." VALUES (".
				"'".$kojin_num."',".
				"'".$name_s."',".
				"'".$name_n."',".
				"'".$kana_s."',".
				"'".$kana_n."',".
				"'".$zip1."',".
				"'".$zip2."',".
				"'".$prf."',".
				"'".$city."',".
				"'".$add1."',".
				"'".$add2."',".
				"'".$tel1."',".
				"'".$tel2."',".
				"'".$tel3."',".
				"'".$fax1."',".
				"'".$fax2."',".
				"'".$fax3."',".
				"'".$email."',".
				"'".$email."',".
				"'2',".
				"'0',".
				"'2',".
				"'".$kei1."',".
				"'".$kei2."',".
				"'".$kei3."',".
				"'".$email2."');";
//echo($sql."<br>");
			$sql1 = pg_query(DB,$sql);
			$check = pg_num_rows($sql1);
		if ($check < 0) {
			$ERROR[] = "お客様情報が記録できませんでした。(".$kojin_num.")";
		}

	}

	//	注文チェック登録
	$sql =  "INSERT INTO nopoints VALUES (" .
			"'".$kojin_num."'," .
			"'".$sells_num."');";

	$sql1 = pg_query(DB,$sql);
	$check = pg_num_rows($sql1);

	if ($check < 0) {
		$ERROR[] = "注文チェックが記録できませんでした。";
	}

	//	届け先住所登録
	//	add_num取り出し
	$sql = "SELECT MAX(add_num) FROM add;";
	$sql1 = pg_query(DB,$sql);
	list($add_num) = pg_fetch_array($sql1,0);
	$add_num = $add_num + 1;

	//	記録
	if (!$g_point) {
		$g_point = "0";
	}
	$sql =  "INSERT INTO add VALUES (".
			"'".$add_num."',".
			"'".$kojin_num."',".
			"'".$name_s."',".
			"'".$name_n."',".
			"'".$kana_s."',".
			"'".$kana_n."',".
			"'".$zip1."',".
			"'".$zip2."',".
			"'".$prf."',".
			"'".$city."',".
			"'".$add1."',".
			"'".$add2."',".
			"'".$tel1."',".
			"'".$tel2."',".
			"'".$tel3."',".
			"'".$fax1."',".
			"'".$fax2."',".
			"'".$fax3."',".
			"'".$email."',".
			"'".$zaiko."',".
			"'".$shiharai."',".
			"'".$haitatu."',".
			"'".$msr."',".
			"'".$g_point."',".
			"'".$toi_num."',".
			"'".$kei1."',".
			"'".$kei2."',".
			"'".$kei3."',".
			"'".$email2."',".
			"'0',".
			"'".$_SESSION['TOKEN']."',".	//	add ookawara 2014/02/25
			"'".$charge_id."'".	//	PAYJP支払い番号
			");";
	$sql1 = pg_query(DB,$sql);
	$check = pg_num_rows($sql1);

	if ($check < 0) {
		$ERROR[] = "送り先が記録できませんでした。(".$add_num.":".$kojin_num.")";
	}

	//	商品登録
	list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS);

//$waribiki = 10;
//$waribiki2 = 30;
//$DISCOUNT_C = 1;
	//	割引率
	if ($idpass && $kojin_num <= $wa_member && $waribiki > 0) {
		$bsell = $waribiki;
	} elseif (!$idpass && $waribiki2 > 0) {
		$bsell = $waribiki2;
	} else {
		$bsell = "";
	}
	$price_all = 0;
	$goods = "";
	$goods_a = "";
	$souryou_muryou_flag = 0;
	if ($KAGOS) {
		foreach ($KAGOS AS $val) {
			$syoukei = 0;
			list($hinban_,$title_,$kakaku_,$num_) = preg_split("/::/",$val);
			if ($hinban_ == "") {
				continue;
			}
			//	送料無料チェック
			$hinban_id = "";
			$GDNM = explode("-",$hinban_);
			$gdnm_max = count($GDNM) - 1;
			for ($i=0; $i<$gdnm_max; $i++) {
				if ($hinban_id) {
					$hinban_id .= "-";
				}
				$hinban_id .= $GDNM[$i];
			}
			if ($SOURYOU_MURYOU[$hinban_id]) {
				$souryou_muryou_flag = 1;
			}

			$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_t * $num_;
			$tanka = number_format($kakaku_t);
			$price_all = $price_all + $syoukei;
			$kakaku_h = number_format($kakaku_);
			$syoukei = number_format($syoukei);
			$title_ = preg_replace("/・/"," ・ ",$title_);

			//	商品の登録
			if (!$bsell) {
				$bsell = "0";
			}
			if (!$P_RITU) {
				$P_RITU = "0";
			}
			$sql =  "INSERT INTO ".T_SELLS." VALUES (".
					"'".$sells_num."',".
					"'".$kojin_num."',".
					"'".$add_num."',".
					"'".$hinban_."',".
					"'".$title_."',".
					"'".$kakaku_."',".
					"'".$num_."',".
					"'0',".
					"'1000-01-01',".
					"'".$bsell."',".
					"'".$P_RITU."',".
					"'".$TAX_."');";
			$sql1 = pg_query(DB,$sql);
			$check = pg_num_rows($sql1);

			if ($check < 0) {
				$ERROR[] = "商品が記録できませんでした。(".$hinban_.":".$kojin_num.")";
			}

			//	在庫商品名追加
			$ERRORS = zaiko_add($hinban_,$title_,$kakaku_);
			if ($ERRORS) {
				$ERROR[] = $ERRORS;
			}

			//	メール書き込み部分
			if ($goods_a) {
				$goods_a .= "\n";
			}

			$goods_a .= <<<EOT
商品番号：{$hinban_}
商品名：{$title_}
単価：{$tanka} 円
数量：{$num_}
小計：{$syoukei} 円
--------------------------------------------------------
EOT;

			$card_goods[] = $title_."(".$hinban_.") 数量:".$num_;

			//	A8用
			$_SESSION['a8']['si']['$a8_num'] = $kakaku_t.$num_.$syoukei.$hinban_;
			$a8_num += 1;

			//	売りリスト
			$sql = "SELECT buy_n FROM total".
					" WHERE hinban='$hinban_'".
					" AND s_date='now()';";
			$sql1 = pg_query(DB,$sql);
			$check = pg_num_rows($sql1);

			//	新規
			if ($check <= 0) {
				if (!$num_) { $num_ = "0"; }
				$sql =  "INSERT INTO total VALUES (" .
						"'".$hinban_."'," .
						"'".$title_."'," .
						"'".$num_."'," .
						"'now()');";
				$sql1 = pg_query(DB,$sql);
				$check = pg_num_rows($sql1);
				if ($check < 0) {
					$ERROR[] = "売り上げ商品を登録することができませんでした。(".$hinban_.")";
				}

			//	追加
			} else {
				list($buy_n) = pg_fetch_array($sql1,0);
				$buy_n = $num_ + $buy_n;
				$sql =  "UPDATE total SET " .
						" buy_n='".$buy_n."'" .
						" where hinban='".$hinban_."' AND s_date='now()';";
				$sql1 = pg_query(DB,$sql);
				$check = pg_num_rows($sql1);
				if ($check < 0) {
					$ERROR[] = "売り上げ商品を追加登録することができませんでした。(".$hinban_.")";
				}
			}
		}
	}

	if ($OPTIONS) {
		foreach ($OPTIONS AS $val) {
			$syoukei = 0;
			$goods_b = "";
##			list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = split("::",$val);
			list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);
			if ($hinban_ == "") {
				continue;
			}

			if ($goods_a) {
				$goods_a .= "\n";
			}

			$goods_b = <<<EOT
マーキング
マーキング商品番号：{$hinban_}
マーキング商品名：{$title_}

EOT;

			$card_goods[] = "マーキング:".$title_."(".$hinban_.")";

			//	持ち込み手数料
			if ($hinban_ == "mochikomi") {
				$kakaku_ = $mochi_pri;
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei_ = $syoukei = $kakaku_t;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= "持ち込み手数料：".$syoukei." 円\n\n";

				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t."1".$syoukei_.$hinban_;
				$a8_num += 1;

			}

			//	背番号
			if ($seban_l_) {
				$moji_num = strlen($seban_num_);
				$kakaku_ = $SEBAN_P_N[$seban_l_];
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$tanka = number_format($kakaku_t);
				$seban_price = $kakaku_ * $moji_num;
				$syoukei_ = $syoukei = $kakaku_t * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= <<<EOT
背番号：{$SEBAN_N[$seban_l_]}
番号：{$seban_num_}
単価：{$tanka} 円
数量：{$moji_num}
小計：{$syoukei} 円


EOT;
				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t.$moji_num.$syoukei_."SEBAN-".$seban_l_;
				$a8_num += 1;
			}

			//	背ネーム
			if ($sename_l_) {
				$sename_name_ = str_replace('\\', '', $sename_name_);
				$sename_name_m = str_replace(' ', '', $sename_name_);
				$moji_num = strlen($sename_name_m);
				$kakaku_ = $SENAME_P_N[$sename_l_];
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$tanka = number_format($kakaku_t);
				$sename_price = $kakaku_ * $moji_num;
				$syoukei_ = $syoukei = $kakaku_t * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= <<<EOT
背ネーム：{$SENAME_N[$sename_l_]}
ネーム：{$sename_name_}
単価：{$tanka} 円
数量：{$moji_num}
小計：{$syoukei} 円


EOT;
				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t.$moji_num.$syoukei_."SENAME-".$sename_l_;
				$a8_num += 1;
			}

			//	胸番号
			if ($muneban_l_) {
				$moji_num = strlen($muneban_num_);
				$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$tanka = number_format($kakaku_t);
				$muneban_price = $kakaku_ * $moji_num;
				$syoukei_ = $syoukei = $kakaku_t * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= <<<EOT
胸番号：{$MUNEBAN_N[$muneban_l_]}
番号：{$muneban_num_}
単価：{$tanka} 円
数量：{$moji_num}
小計：{$syoukei} 円


EOT;
				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t."1".$syoukei_."MUNEBAN-".$muneban_l_;
				$a8_num += 1;
			}

			//	パンツ番号
			if ($pant_l_) {
				$moji_num = strlen($pant_num_);
				$kakaku_ = $PANT_P_N[$pant_l_];
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$tanka = number_format($kakaku_t);
				$pant_price = $kakaku_ * $moji_num;
				$syoukei_ = $syoukei = $kakaku_t * $moji_num;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= <<<EOT
パンツ番号：{$PANT_N[$pant_l_]}
番号：{$pant_num_}
単価：{$tanka} 円
数量：{$moji_num}
小計：{$syoukei} 円


EOT;
				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t."1".$syoukei_."PANT-".$pant_l_;
				$a8_num += 1;
			}

			//	バッジ
			if ($bach_l_) {
				$kakaku_ = $BACH_P_N[$bach_l_];
				$kakaku_t = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$bach_price = $kakaku_;
				$syoukei_ = $syoukei = $kakaku_t;
				$price_all = $price_all + $syoukei;
				$kakaku_ = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$goods_b .= <<<EOT
バッジ：{$BACH_N[$bach_l_]}
金額：{$syoukei} 円


EOT;
				//	A8用
				$_SESSION['a8']['si']['$a8_num'] = $kakaku_t."1".$syoukei_."BACH-".$bach_l_;
				$a8_num += 1;
			}

			$goods_a .= <<<EOT
{$goods_b}
--------------------------------------------------------
EOT;

			//	商品の登録
			if (!$seban_l_) {
				$seban_l_ = "0";
			}
			if (!$seban_price) {
				$seban_price = "0";
			}
			if (!$sename_l_) {
				$sename_l_ = "0";
			}
			if (!$sename_price) {
				$sename_price = "0";
			}
			if (!$muneban_l_) {
				$muneban_l_ = "0";
			}
			if (!$muneban_price) {
				$muneban_price = "0";
			}
			if (!$pant_l_) {
				$pant_l_ = "0";
			}
			if (!$pant_price) {
				$pant_price = "0";
			}
			if (!$bach_l_) {
				$bach_l_ = "0";
			}
			if (!$bach_price) {
				$bach_price = "0";
			}

			$sql =  "INSERT INTO option VALUES (" .
					"'".$op_num_."'," .
					"'".$sells_num."'," .
					"'".$kojin_num."'," .
					"'".$hinban_."'," .
					"'".$title_."'," .
					"'".$seban_l_."'," .
					"'".$seban_num_."'," .
					"'".$seban_price."'," .
					"'".$sename_l_."'," .
					"'".$sename_name_."'," .
					"'".$sename_price."'," .
					"'".$muneban_l_."'," .
					"'".$muneban_num_."'," .
					"'".$muneban_price."'," .
					"'".$pant_l_."'," .
					"'".$pant_num_."'," .
					"'".$pant_price."'," .
					"'".$bach_l_."'," .
					"'".$bach_name."'," .
					"'".$bach_price."'," .
					"'0'," .
					"'1000-01-01');";
			$sql1 = pg_query(DB,$sql);

			if (!$op_num_) {
				$op_num_ = "0";
			}
			if (!$bsell) {
				$bsell = "0";
			}
			$sql =  "INSERT INTO ".T_SELLS." VALUES (" .
					"'".$sells_num."'," .
					"'".$kojin_num."'," .
					"'".$add_num."'," .
					"'option',".
					"'".$op_num_."'," .
					"'0'," .
					"'0'," .
					"'0'," .
					"'1000-01-01'," .
					"'".$bsell."',".
					"'".$P_RITU."',".
					"'".$TAX_."');";
			$sql1 = pg_query(DB,$sql);
			$check = pg_num_rows($sql1);
			if ($check < 0) {
				$ERROR[] = "商品が記録できませんでした。($op_num_:$kojin_num)";
			}
		}
	}

	//	特別会員割引
	if ($idpass && $bsell > 0) {
		$nebiki = $price_all * $waribiki / 100;
		$price_all = $price_all - $nebiki;
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		$bsell_m = <<<EOT

特別会員割引( {$waribiki} % )
-{$nebiki} 円
--------------------------------------------------------
EOT;

	} elseif (!$idpass && $bsell > 0) {
		$nebiki = $price_all * $waribiki2 / 100;
		$price_all = $price_all - $nebiki;
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		$bsell_m = <<<EOT

割引( {$waribiki2} % )
-{$nebiki} 円
--------------------------------------------------------
EOT;
	} elseif ($DISCOUNT_C == 1) {
		unset($nebiki);
		$DISCOUNT = array_reverse($DISCOUNT);
		foreach ($DISCOUNT AS $VAL) {
			$totalprice_ = $VAL[0];
			$paersent_ = $VAL[1];
			if ($price_all > $totalprice_) {
				$paersent = $paersent_;
				break;
			}
		}
		$nebiki = $price_all * $paersent / 100;
		$price_all = $price_all - $nebiki;
		$nebiki = floor($nebiki);	// add yoshizawa 2014/01/06
		$nebiki = number_format($nebiki);

		if ($nebiki) {
			$bsell_m = <<<EOT

割引( {$paersent} % )
-{$nebiki} 円
--------------------------------------------------------
EOT;

			$sql  = "UPDATE ".T_SELLS." SET".
					" bargain='".$paersent."'".
					" WHERE sells_num='".$sells_num."';";
			$sql1 = pg_query(DB,$sql);

		}
	}

	//	アフェリエイト金額決定
	$aff_price = $price_all;

	//	A8用
	$_SESSION['a8']['si']['0'] = $aff_price."1".$aff_price."all";

	//	ポイント減
	if ($r_point == 1 && $g_point) {
		//	ポイント確認
		$sql =  "SELECT point FROM ".T_KOJIN.
				" WHERE kojin_num='".$kojin_num."'".
				" AND saku!='1';";
		$sql1 = pg_query(DB,$sql);
		$check = pg_numrows($sql1);
		if ($check < 0) {
			$ERROR[] = "ポイント確認できませんでした。";
		}
		list($point) = pg_fetch_array($sql1,0);
		$point = $point - $g_point;

		if ($point <= 0) {
			$point = 0;
		}

		$sql =  "UPDATE ".T_KOJIN.
				" SET  point='".$point."'".
				" WHERE kojin_num='".$kojin_num."'".
				" AND saku!='1';";
		$sql1 = pg_query(DB,$sql);
		$check = pg_num_rows($sql1);
		if ($check < 0) {
			$ERROR[] = "ポイントを減らすことができませんでした。";
		}

	}



	//	メール作業
	$ip = getenv("REMOTE_ADDR");
	$host = gethostbyaddr($ip);
	if (!$host) {
		$host = $ip;
	}

	if ($r_point == 1 && $g_point) {
		$p_all = $price_all;
		$g_point_p = $g_point;
		$p_all = $p_all - $g_point_p;
		if ($p_all <= 0) {
			$g_point_p = $g_point_p + $p_all;
			$g_point = ceil($g_point_p);
			$price_all = 0;
		} else {
			$price_all = $p_all;
		}
	}

	$k_point = $price_all * ($P_RITU / 100);
	$k_point = floor($k_point + 0.5);

	$unchin = $UN_N[$prf];
	$unchin = floor($unchin * ($TAX_ + 1) + 0.5);

	$sevice_check = 0;
	if ($free_shipping != "" && $free_shipping <= $price_all) {
		unset($unchin);
		$sevice_check = 1;
	} elseif ($souryou_muryou_flag == 1) {
		unset($unchin);
	}

	if ($shiharai == 1) {
//		$TESU_P = floor($TESU_P * ($TAX_ + 1) + 0.5);
//		$price_all = $price_all + $TESU_P + $unchin;
		$price_all = $price_all + $unchin;
		$max = count($DAIBIKI_N);
		$daibiki = "";
		for($i=0; $i<$max; $i++) {
			$p_all = $price_all;
			//	支払金額が０の場合
			if ($p_all < 1) {
				$daibiki = "";
				$tax = "";
				$price_all = "";
				break;
			}

			if (!$daibiki && $DAIBIKI_N[$i] >= $p_all) {
				$daibiki = $DAIBIKI_P_N[$i];
				$daibiki = floor($daibiki * ($TAX_ + 1) + 0.5);

				$p_all = $p_all + $daibiki;
//				$tax = $p_all * $TAX_ + 0.5;
//				$tax = floor($tax);
//				$p_all = $p_all + $tax;
				if ($DAIBIKI_N[$i] >= $p_all) {
					$price_all = $p_all;
				} else {
					$daibiki = "";
					$tax = "";
				}
			}
		}
	} elseif ($shiharai == 4) {
		$con_tesu = floor($CON_TESU * ($TAX_ + 1) + 0.5);
		$price_all = $price_all + $unchin + $con_tesu;
	//	後払い手数料計算	add ookawara 2010/12/10
	} elseif ($shiharai == 5) {
		$ato_tesu = floor(($price_all+ $unchin) * (atobarai / 100) + 0.5);
		if ($ato_tesu < ato_low_price) {
			$ato_tesu = ato_low_price;
		}	// add ookawara 2011/01/20
		$price_all = $price_all + $unchin + $ato_tesu;
	} else {
		$price_all = $price_all + $unchin;
//		$tax = $price_all * $TAX_ + 0.5;
//		$tax = floor($tax);
//		$price_all = $price_all + $tax;
	}

	$g_point_p = number_format($g_point_p);
	$k_point = number_format($k_point);
//	$tax = number_format($tax);
	$daibiki = number_format($daibiki);
	$tesu = number_format($tesu);
	$unchin_ = $unchin;
	$unchin = number_format($unchin);
	$price_all_ = $price_all;
	$price_all = number_format($price_all);

	if ($r_point == 1 && $g_point) {
		$point_m = <<<EOT

割引ポイント利用
-{$g_point_p} 円
--------------------------------------------------------
EOT;
	}

	if ($shiharai == 1) {
		$shiharai_m = <<<EOT
代引手数料
{$daibiki} 円
--------------------------------------------------------
EOT;

		$shiharai_m1 = <<<EOT
支払手数料
{$TESU_P} 円
--------------------------------------------------------
EOT;
	} elseif ($shiharai == 4) {
		$shiharai_m = <<<EOT
請求書発行手数料
{$con_tesu} 円
--------------------------------------------------------
EOT;

		$shiharai_m1 = <<<EOT
請求書発行手数料
{$CON_TESU} 円
--------------------------------------------------------
EOT;
	//	後払い手数料	add ookawara 2010/12/10
	} elseif ($shiharai == 5) {
		$shiharai_m = <<<EOT
後払い決済手数料
{$ato_tesu} 円
--------------------------------------------------------
EOT;

		$shiharai_m1 = <<<EOT
後払い決済手数料
{$ato_tesu} 円
--------------------------------------------------------
EOT;
	}

//	if($kojin_num<=100000 && $idpass && $r_point != 1) {
	if($kojin_num<=100000 && $idpass && $k_point) {
		$k_point_m = <<<EOT
--------------------------------------------------------
獲得予定割引ポイント
{$k_point} pt

EOT;
	}


	$tax_p = $TAX_ * 100;

	if ($tel1 && $tel2 && $tel3) {
		$TEL = "固定電話：".$tel1."-".$tel2."-".$tel3;
	}
	if ($tel1 && $tel2 && $tel3 && $kei1 && $kei2 && $kei3) {
		$TEL .= "\n　";
	}
	if ($kei1 && $kei2 && $kei3) {
		$KEI = "携帯電話：".$kei1."-".$kei2."-".$kei3;
	}

	$FAX = $fax1."-".$fax2."-".$fax3;

	$card_msg1 = "";
	$card_msg2 = "";
	if ($shiharai == 3) {
		$card_url = "/cago.php?num=ORG-".$sells_num;
		$card_msg1 = <<<EOT
クレジットカード決済
{$card_url}
########################################################

EOT;
	$card_msg2 = <<<EOT
クレジットカード決済ご利用有り難う御座います。
決済のお手続きが済んでいない場合以下のアドレスから
お手続きお願い致します。
クレジット決済完了されても、発送しない限り請求される事はございません。
{$card_url}
########################################################

EOT;
	} elseif ($shiharai == 4) {
	$card_msg2 = <<<EOT
(前払い)コンビニ払いご利用有り難う御座います。
お振り込み用紙（お支払い用紙）を、商品お送り先住所に
お送りさせていただきますのでお待ちください。
お支払いが確認でき次第商品を発送させていただきます。
########################################################

EOT;
	//	後払い手数料	add ookawara 2010/12/10
	} elseif ($shiharai == 5) {
	$card_msg2 = <<<EOT
後払い決済ご利用有り難う御座います。
商品到着後14日以内にコンビニ/銀行/郵便局でお支払いお願いします。
「後払い決済」に関して詳しい内容は以下のURLをご覧ください。
http://www.ato-barai.com/annai.html
########################################################

EOT;
	}



	//	送料情報
	if ($sevice_check == 1 || $souryou_muryou_flag == 1) {
		$unchin_msg = "サービス";
	} else {
		$unchin_msg = $unchin." 円";
		$card_goods[] = "送料";
	}
	if (!$unchin_) {
		$unchin_ = "0";
	}

	$sql =  "UPDATE add SET shipping='".$unchin_."'".
			" WHERE add_num='".$add_num."';";
	$sql1 = pg_query(DB,$sql);



	//	受注メール送信
	$error = "";

	if ($ERROR) {
		foreach ($ERROR AS $val) {
			$error .= "・".$val." \n";
		}
		if ($error) {
			$error = <<<EOT
--------------------------------------------------------
{$error}
EOT;
		}
	}

	$subject = "注文番号[ ORG-".$sells_num." ] ".$name_s." 様( No.".$kojin_num." )からのご注文です。";
	$msg = <<<EOT
{$subject}
########################################################
{$card_msg1}
ご注文内容

--------------------------------------------------------
{$goods_a}{$bsell_m}{$point_m}
送料
{$unchin_msg}
--------------------------------------------------------
{$shiharai_m}
合計金額
{$price_all} 円
{$k_point_m}
########################################################

送り先情報

--------------------------------------------------------

氏名
　{$name_s} {$name_n}

ふりがな
　{$kana_s} {$kana_n}

住所
　〒{$zip1}-{$zip2}
　{$PRF_N[$prf]} {$city} {$add1} {$add2}

電話番号
　{$TEL}{$KEI}

FAX番号
　{$FAX}

メールアドレス
　{$email}{$EMAIL2}

お届け時間
　{$HAITATU_N[$haitatu]}

在庫無き場合
　{$ZAIKO_N[$zaiko]}

支払方法
　{$SHIHARAI_N[$shiharai]}

------------------------------------------------------
ご意見ご要望
　{$msr}

########################################################

{$error}
EOT;
//echo "msg=".$msg."<br />";

	//メール送信処理
	$send_email = $email;
	$send_name = $name_s.$name_n;
	$get_email = $admin_mail;
//$get_email = "検証アドレス";
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);

	//	確認メール送信
	$subject = "ご注文ありがとうございます。  - ネイバーズスポーツ -";
	$msg = <<<EOT
{$subject}
{$name_s} 様 ご購入内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがご連絡お願いします。
########################################################
{$card_msg2}
ご注文内容 (ご注文番号：ORG-{$sells_num})

--------------------------------------------------------
{$goods_a}{$bsell_m}{$point_m}
送料
{$unchin_msg}
--------------------------------------------------------
{$shiharai_m}
合計金額
{$price_all} 円
{$k_point_m}
########################################################

送り先情報

--------------------------------------------------------

氏名
　{$name_s} {$name_n}

ふりがな
　{$kana_s} {$kana_n}

住所
　〒{$zip1}-{$zip2}
　{$PRF_N[$prf]} {$city} {$add1} {$add2}

電話番号
　{$TEL}{$KEI}

FAX番号
　{$FAX}

メールアドレス
　{$email}{$EMAIL2}

お届け時間
　{$HAITATU_N[$haitatu]}

在庫無き場合
　{$ZAIKO_N[$zaiko]}

支払方法
　{$SHIHARAI_N[$shiharai]}

------------------------------------------------------
ご意見ご要望
　{$msr}

########################################################

{$m_footer}
EOT;
//echo "msg=".$msg."<br />";

	//メール送信処理
	$send_email = $admin_mail;
	$send_name = $admin_name;
	$get_email = $email;
//$get_email = "検証アドレス";
	send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg);


	//	アフェリエイト登録
//	if (!$_SESSION['affid']) {
//		$_SESSION['affid'] = 1;	//	後で消す
//	}

	if (!$ERROR && $_SESSION['affid'] && $aff_price > 0) {
		//	$af_num		アフェリエイトid
		//	$sells_num	注文番号
		//	$p_ritu		アフェリエイト率
		//	$point		ポイント

		$af_num = $_SESSION['affid'];
		$p_ritu = $aff_ritsu;
		$point = floor($aff_price * $p_ritu / 100);

		$sql  = "INSERT INTO ".T_APPOINT.
				" (af_num,sells_num,p_ritu,point,order_day)".
				" VALUES(".
				"'".$af_num."',".
				"'".$sells_num."',".
				"'".$p_ritu."',".
				"'".$point."',".
				"now());";
		$result = pg_query(DB,$sql);

	}
	// PAYJP 支払い更新
	if ($shiharai==7){
	updatePayjpChargeDescription($_SESSION['payjp']['charge_id'],"支払い完了コメント");
	$metadata=array(
		"kojin_num"=>$kojin_num,
		"add_num"=>$add_num,
	);
	updatePayjpChargeMetadata($_SESSION['payjp']['charge_id'],$metadata);
	}

	//	セッション解除
	unset($customer);
	unset($_SESSION['customer']);
	unset($addr);
	unset($_SESSION['addr']);
	unset($opt);
	unset($_SESSION['opt']);
	unset($point);
	unset($_SESSION['point']);
	unset($affid);
	unset($_SESSION['affid']);
	unset($_SESSION['payjp']); //　PAYJP支払データ取消
	unset($_SESSION['paypal_list']);	//	add	ookawara	2014/01/24

	setcookie("affid","$affid",time(),"/",".futboljersey.com");
	unset($_COOKIE['affid']);
	setcookie("affid");

	//	カード用データー保存
	if ($shiharai == 3) {
		$order_num = "ORG-" . $sells_num;
		$price = preg_replace("[^0-9]","",$price_all);
		$name = $name_s."　".$name_n;
		$tel = preg_replace("[^0-9]","",$TEL);
		$email = $email;
		unset($goods_name);

		if ($card_goods) {

			foreach ($card_goods AS $val) {
				if ($goods_name) {
					$goods_name .= " // ";
				}
				$goods_name .= $val;

			}
			//$goods_name = preg_replace(" // $","",$goods_name);
			$goods_name = preg_replace(" // ","",$goods_name);/* 2013/09/04 add yoshizawa */
		}

		if (strlen($goods_name) > 400) {
			$goods_name = $card_goods[0]." // 他";
		}

		$_SESSION['CARD']['order_num'] = $order_num;
		$_SESSION['CARD']['goods_name'] = $goods_name;
		$_SESSION['CARD']['price'] = $price;
		$_SESSION['CARD']['name'] = $name;
		$_SESSION['CARD']['tel'] = $tel;
		$_SESSION['CARD']['email'] = $email;


		$sql  = "INSERT INTO card".
			"(order_num, goods_name, price, name, tel, email, regist_date)".
			" VALUES(".
			"'".$order_num."',".
			"'".$goods_name."',".
			"'".$price."',".
			"'".$name."',".
			"'".$tel."',".
			"'".$email."',".
			"now());";
		$sql1 = pg_query(DB,$sql);

	}

	//	thanks画面に飛ばす。
	//	add ookawara 2014/04/03
	$url = THANKSURL;
	header ("Location: $url\n\n");
	exit;

	//	PayPal支払い時、最終支払いセット
	//	add ookawara 2014/02/25
	//last_set_paypal();

	//pg_close(DB);	//	del ookawara 2014/02/25

}



	//	在庫商品名追加
function zaiko_add($hinban_,$title_,$kakaku_) {

	$sql =  "SELECT hinban FROM zaiko".
			" WHERE hinban='".$hinban_."';";
	$sql1 = pg_query(DB,$sql);
	$check = pg_num_rows($sql1);
	if ($check <= 0) {
		$sql = "SELECT MAX(zaiko_num) FROM zaiko;";
		$sql1 = pg_query(DB,$sql);
		list($zaiko_num) = pg_fetch_array($sql1,0);
		$zaiko_num = $zaiko_num + 1;

		if (!$kakaku_) {
			$kakaku_ = "0";
		}
		if (!$stock) {
			$stock = "0";
		}
		$sql =  "INSERT INTO zaiko VALUES (".
				"'".$zaiko_num."',".
				"'".$hinban_."',".
				"'".$title_."',".
				"'".$size."',".
				"'".$category1."',".
				"'".$category2."',".
				"'".$category3."',".
				"'".$kakaku_."',".
				"'".$stock."');";
		$sql1 = pg_query(DB,$sql);
		$check = pg_num_rows($sql1);
		if ($check < 0) {
			$ERROR[] = "商品が記録できませんでした。(".$hinban_.")";
		}
	}

	return $ERROR;

}



	//	カード払い処理＆お礼メッセージ
function cago_thank() {


	//	加盟店コード
	$code = "820607901";

	//	A8 プログラムID
	$a8_pid = "s00000005771001";

	//	手続き先url
	$send_url = "http://www/";

	if ($_SESSION['CARD']) {

		$order_num = $_SESSION['CARD']['order_num'];
		$goods_name = $_SESSION['CARD']['goods_name'];
		$price = $_SESSION['CARD']['price'];
		$name = $_SESSION['CARD']['name'];
		$tel = $_SESSION['CARD']['tel'];
		$email = $_SESSION['CARD']['email'];
	} elseif (preg_match("/^ORG/", $_GET["num"])) {
		$sql =  "SELECT * FROM card".
				" WHERE order_num='".$_GET["num"]."'".
				" ORDER BY regist_date".
				" DESC LIMIT 1;";
		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$order_num = $list["order_num"];
			$goods_name = $list["goods_name"];
			$price = $list["price"];
//			$name = $list['name'];
//			$tel = $list['tel'];
//			$email = $list['email'];
		}
	}

	if ($_SESSION['a8']) {
		$so = $_SESSION['a8']['so'];
		$si = $_SESSION['a8']['si'][0];
		$ip = getenv("REMOTE_ADDR");	//	getenv 環境変数の値を取得する
		$ip = preg_replace("/\./","-",$ip);
		$a8_img = "<img src=\"https://px.a8.net/cgi-bin/a8fly/sales?pid=".$a8_pid."&so=".$so."-".$ip."&si=".$si."\" width=\"1\" height=\"1\"><br />\n";
	}

	$flag = 0;
	if (!$order_num && !$goods_name && !$price) {
		$DEL_INPUTS['CARDMSG'] = 1;		//	カード支払
	}

	//	カード支払情報＆アフィリエイト情報削除
	unset($_SESSION['CARD']);
	unset($_SESSION['a8']);


	//	PayPalセッション削除
	//	add ookawara 2014/02/25
	unset($_SESSION['paypal_list']);
	// PAYJP データ削除
	unset($_SESSION['payjp']);
	unset($_SESSION['payer_id']);
	unset($_SESSION['curl_error_no']);
	unset($_SESSION['curl_error_msg']);
	unset($_SESSION['nvpReqArray']);
	unset($_SESSION['TOKEN']);
	unset($_SESSION['currencyCodeType']);
	unset($_SESSION['PaymentType']);
	unset($_SESSION['Payment_Amount']);


	$INPUTS['A8IMG'] = $a8_img;			//	アフィリエイト

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("thank_c.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;

}


//	メール配信サブルーチン
function send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msg) {

	//	文字コード宣言
	mb_language("Japanese");
	mb_internal_encoding("UTF-8");
	$send_name = header_base64_encode($send_name);
	$from = "From: ".$send_name." <".$send_email.">\nReply-To: ".$send_email."\n";
	if ($mail_bcc == 1) {
		$from .= "Bcc: ".$bcc_email."\n";
	}

	//	文字化け対策
	$subject = "　" . $subject;

	mb_send_mail ( $get_email, $subject, $msg , $from , "-f$send_email");

}
function header_base64_encode($str) {

	$result = iconv("UTF-8", "ISO-2022-JP", $str).chr(27).'(B';	//iconv 文字列を指定した文字エンコーディングに変換する
	$result = '=?ISO-2022-JP?B?'.base64_encode($result).'?=';

	return $result;

}
