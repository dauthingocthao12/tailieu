<?PHP

include_once('./sub/souryou.inc');	//	add ookawara 2016/07/28

//	商品一覧
function goodsname($VALUE,$CHECK) {
	global $db,$CLASS_L,$LOG_DIR,$view2,$gpx,$gpy,$tax,$h_tax,$index;

	//	カート内の在庫数セット	//	add ookawara 2010/02/01
	$CART = array();
	if ($_SESSION['customer']) {
		$CART = $_SESSION['customer'];
	}

	//	送料無料設定定数変数設定
	//	add ookawara 2016/07/28
	$souryoufree = 0;
	if (defined("SOURYOUFREE")) {
		$souryoufree = SOURYOUFREE;
	}
	$souryoufreeprice = 0;
	if (defined("SOURYOUFREEPRICE")) {
		$souryoufreeprice = SOURYOUFREEPRICE;
	}

	$file = "gl";
	$URL = "/goods";
	foreach ($VALUE AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		if ($file) { $file .= "_"; }
		if ($URL) { $URL .= "/"; }
		$file .= $VAL;
##		if (!eregi(".htm",$VAL)) {
		if (!preg_match("/\.htm/i",$VAL)) {
			$URL .= $VAL;
		}
	}
##	if (eregi(".htm",$file)) {
	if (preg_match("/\.htm/i",$file)) {
		$htm = "";
	} else {
		$htm = ".htm";
	}
	$file = "./$LOG_DIR/$file$htm";

##	$URL = eregi_replace("/$","",$URL);
	$URL = preg_replace("/\/$/i","",$URL);

	//	ページ
##	if (eregi("^index+([0-9])+\\.htm",$VAL)) {
	if (preg_match("/^index+([0-9])+\\.htm/i",$VAL)) {
##		$page = eregi_replace("[^0-9]","",$VAL);
		$page = preg_replace("/[^0-9]/i","",$VAL);
	}
	if  (!$page) { $page = 1; }

/*
	//	del ookawara 2014/12/18
	if (file_exists($file)) {
		$fp = fopen ($file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}
	else {
*/
		//	商品名抜き出し
		if ($CHECK[n]) {
##			$list_num = eregi_replace("^n","",$CHECK[n]);
			$list_num = preg_replace("/^n/i","",$CHECK[n]);
			//	2009/04/25	del ookawara
			//$where = " AND goods_name=(SELECT goods_name FROM list WHERE list_num='$list_num' LIMIT 1)";
			$where = " AND list.goods_name=(SELECT goods_name FROM list WHERE list_num='$list_num' AND state!='1' LIMIT 1)";
		}
		//	表示確認
		if ($CHECK[m]) {
##			$maker_num = eregi_replace("^m","",$CHECK[m]) * 1;
			$maker_num = preg_replace("/^m/i","",$CHECK[m]) * 1;
			$where .= " AND maker_num='$maker_num'";
		}
		if ($CHECK[s]) {
##			$class_m = eregi_replace("^s","",$CHECK[s]);
			$class_m = preg_replace("/^s/i","",$CHECK[s]);
			$where .= " AND list.class_m='$class_m'";
		}
		elseif ($CHECK[main]) {
			$snum = $CHECK[main];
			$fnum = $snum + 99;
			$where .= " AND list.class_m::int BETWEEN $snum AND $fnum";
		}

		$sql  = "SELECT count(*) AS count FROM list" .
				" WHERE display='2'".
				" AND state!='1'".		//	2009/04/25	add ookawara
				"$where;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$max = $list[count];
		}
		if ($max > 0) {
			$max_page = ceil($max/$view2);
			if ($max_page < $page) { $page = $max_page; }
			$s = $view2 * ($page - 1);
			$e = ($view2 * $page) - 1;
			if ($e >= $max) { $e = $max - 1; }
			$view_s = $s + 1;
			$view_e = $e+1;

			$page_msg = "全" . $max . "件中 " . $view_s . "-" . $view_e . "件目 ";
			if ($max_page > 1) { $page_msg .= $page . "/" . $max_page . "Page"; }

			$html = <<<WAKABA
			<section class="details-list">
				<h2 class="sub-title-prod">商品一覧</h2>

WAKABA;

			if ($max_page > 1) {
				$html .= <<<WAKABA
				<div class="pagination">$page_msg</div>

WAKABA;
			}

			if ($list_num) {
				$sql  = "SELECT class_m FROM list" .
						" WHERE list_num='$list_num'".
						" AND state!='1'".		//	2009/04/25	add ookawara
						" LIMIT 1;";
				if ($result = pg_query($db,$sql)) {
					$list = pg_fetch_array($result);
					$class_m = $list[class_m];
				}
			}
			if ($class_m >= 500 && $class_m < 600) {
				$size_msgs = "種類1";
				$color_msgs = "種類2";
			} else {
				$size_msgs = "サイズ";
				$color_msgs = "カラー";
			}

			//	商品読み込み
			$offset = ($page - 1) * $view2;
			$limit_max = $offset + $view2;
			if ($max < $limit_max) {
				$limit = $max % $view2;
			} else {
				$limit = $view2;
			}
			$limit_num = " LIMIT $limit";
			if ($offset != 0) {
				$limit_num .= " OFFSET $offset";
			}

			$sql  = "SELECT list.list_num, list.pluid, list.goods_name, maker_num, list.size, list.color, picture".
					", free_postage".		//	add ookawara 2016/07/26
                    ", goods.set_flag, goods.class_m".     // add simon 2018-07-05
					" FROM list, goods" .
					" WHERE display='2'$where" .
                    " AND goods.pluid = list.pluid".
					" AND state!='1'".		//	2009/04/25	add ookawara
					" ORDER BY list.goods_name, size, color$limit_num;";
            // print $sql;
			if ($result = pg_query($db,$sql)) {
				$maker_name = "";
				$bgcolors = 1;
				while ($list = pg_fetch_array($result)) {
					$list_num = $list['list_num'];
					$pluid = $list['pluid'];
					$goods_name = $list[goods_name];
					if ($goods_name) { $goods_name = "　" . $goods_name; }
					$maker_num = $list[maker_num];
					$size = $list[size];
					if (!$size) { $size = "--"; }
					$color = $list[color];
					if (!$color) { $color = "--"; }
					$picture = $list[picture];
					$pic_url = "/pic/$picture";
					$free_postage = $list['free_postage'];	//	add ookawara 2016/07/26

					if ($picture && file_exists(".$pic_url")) { $plist = @getimagesize(".$pic_url"); }
					if ($plist[2] > 0) {
						$wid = $plist[0];
						$hig = $plist[1];
						if ($wid > $gpx || $hig > $gpy) {
							$w_ritu = $gpx / $wid;
							$h_ritu = $gpy / $hig;
							if ($w_ritu < $h_ritu) { $ritu = $w_ritu; }
							else { $ritu = $h_ritu; }
							$width_ = $wid * $ritu;
							$height_ = $hig * $ritu;
						} else {
							$width_ = $wid;
							$height_ = $hig;
						}
						$img_msg = "<IMG src=\"$pic_url\" width=\"$width_\" height=\"$height_\" border=\"0\" alt=\"$goods_name\">\n";
					} else {
						$img_msg = "";
					}

					//$sokunou = "<img src=\"/images/sokunou.gif\" />";	//	add ookawara 2010/12/06
					$sokunou = "<span>即納</span>";

					//$kounyu = "<INPUT type=\"image\" src=\"/images/kounyu.gif\" border=\"0\" alt=\"購入\">";
					$kounyu = "<button class='btn btn-cart' type=\"submit\">購入</button>";

					$sql = "SELECT stock, price FROM goods WHERE pluid='$pluid' LIMIT 1;";
					if ($result2 = pg_query($db,$sql)) {
						$list2 = pg_fetch_array($result2);
						//$stock = $list2[stock];	//	del ookawara 2010/02/01
						$stock = $list2[stock] - $CART[$list_num];	//	add ookawara 2010/02/01
						$stock_msg = "";
						if ($stock > 0) {
							$stock_msg = '<SELECT class="input-inline" name="buy_num">';
							$stock_msg .= '<OPTION value="" selected>--</OPTION>';

							if ($stock >= 20) { $smax = 20; } else { $smax = $stock; }
							for($i=1; $i<=$smax; $i++) {
									$stock_msg .= "              <OPTION value='$i'>$i</OPTION>\n";
							}
							$stock_msg .= "      </SELECT>\n";
						} else {
							$stock_msg = "        品切中\n";
							$kounyu = "--";
							$sokunou = "";	//	add ookawara 2010/12/06
						}

						$check_price = 0;	//	add ookawara 2016/07/28
						$price = $list2[price];
						if ($price) {
							if ($h_tax == 1) {
								$price = $price + floor(($price * $tax) + 0.5);
							}
							$check_price = $price;	//	add ookawara 2016/07/28

							$price = number_format($price) . "円";

							$align = "right";
						} else {
							$price = "--";
							$align = "center";
						}
					} else {
						$stock_msg = "        品切中\n";
						$kounyu = "--";
						$price = "--";
						$align = "center";
						$sokunou = "";	//	add ookawara 2010/12/06
					}

					if (!$maket_name) {
						$sql = "SELECT maker_name FROM maker WHERE maker_num='$maker_num' LIMIT 1;";
						$result3 = pg_query($db,$sql);
						$list3 = pg_fetch_array($result3);
						$maker_name = $list3[maker_name];
						if ($maker_name) { $maker_name = "　" . $maker_name; }
					}

					$amari = $bgcolors % 2;
					if ($amari == 1) { $bgcolor = "#f3f3f3"; } else { $bgcolor = "#cccccc"; }

					// add ookawara 2016/07/28
					//	add ohkawara 2020/04/23	送料無料→基本送料無料
					$free_postage_add_mark = "";
					$def_souryou_flg = 0;	//	add ohkawara 2020/04/23
					if ($souryoufree == 1 && ($free_postage > 0 || ($souryoufreeprice > 0 && $souryoufreeprice <= $check_price))) {
						$free_postage_add_mark = <<<WAKABA
							<div class="blinking" style="color: red; font-weight: bold;">
							基本送料無料
							</div>
WAKABA;
						$def_souryou_flg = 1;	//	add ohkawara 2020/04/23
					}


                    // add simon 2018-07-05 >>>
                    $rod_fee_line = "";
                    if(Souryou::prod_has_rod_fee($list)) {
                        $fee = Souryou::cart_rod_shipping_fee(array($list));
                       	//$rod_fee_line = "<div class='product-rod-fee'>基本送料＋{$fee}円</div>";	//	del ohkawara 2020/04/23

						//	add ohkawara 2020/04/23
						if ($def_souryou_flg == 1) {
                        	$rod_fee_line = "<div class='product-rod-fee'>追加送料 ".number_format($fee)."円</div>";
						} else {
                        	$rod_fee_line = "<div class='product-rod-fee'>基本送料 ＋ 追加送料 ".number_format($fee)."円</div>";
						}

                    }
                    // <<<

					$html .= <<<WAKABA
		<div class="product-list item-box clearfix">
			<h3 class="product-name clearfix">
				<A href="$URL/g$list_num/$index">$goods_name (No.$list_num)</A>
				<div class="product-details-link">
					<A class="btn btn-details" href="$URL/g$list_num/$index">詳細</A>
				</div>
			</h3>
			<FORM action="/cart.php" method="POST">
				<INPUT type="hidden" name="list_num" value="$list_num">

				<div class="product-table">
					<div class="product-image">
						<A href="$URL/g$list_num/$index">$img_msg</A>
						$free_postage_add_mark
					</div>
					<div class="product-info">
						<div class="product-maker"><b>メーカー</b>$maker_name</div>
						<div class="product-size"><b>サイズ</b>$size</div>
						<div class="product-color"><b>カラー</b>$color</div>
					</div>
$rod_fee_line
					<div class="product-order">
						<div class="product-instantdelivery">$sokunou&nbsp;</div>
						<div class="product-price"><span>$price</span></div>
						<div class="product-quantity">$stock_msg</div>
						<div class="product-addtocart">$kounyu</div>
					</div>
				</div>
			</FORM>
		</div>

WAKABA;

					$bgcolors++;

				}
			}

			//	ページ処理
			if ($max_page > 1) {
				$html .= "<div class='pagination'>\n";

				if ($page != 1) {
					$page_b = $page-1;
					if ($page_b > 1) {
						$b_url = $URL . "/index" . $page_b . ".htm";
					} else {
						$b_url = "$URL/";
					}

					$html .= "<A class='btn btn-primary' href='$b_url'>前の $view2 件</A>\n";
				}

				if ($max_page != $page && $max_page != 1) {
					$page_n = $page + 1;
					$view_n = $max-($page*$view2);
					if ($view_n > $view2) { $view_n = $view2; } else { $view_n = $view_n; }
					if ($page_n > 1) { $n_url = $URL . "/index" . $page_n . ".htm"; }
					else { $n_url = "$URL/"; }
					$html .= "<A class='btn btn-primary' href='$n_url'>次の $view_n 件</A>\n";
				}

				$html .= "</div>\n"; // end pagination
			}

			$html .= "</section>\n";
		}

/*
	//	del ookawara 2014/12/18
		if ($html == "aa") {
			$OUT = fopen("$file","w");
			fwrite($OUT,$html);
			fclose($OUT);
			@chmod($file,0666);
		}

	}
*/

	return $html;

}
?>
