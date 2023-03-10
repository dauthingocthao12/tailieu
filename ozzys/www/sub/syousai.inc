<?PHP

include_once('./sub/souryou.inc');	//	add ookawara 2016/07/28

//	商品詳細
function syousai($VALUE,$CHECK) {
global $db,$CLASS_L,$LOG_DIR,$view2,$gpx,$gpy,$tax,$h_tax;

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

	$file = "g";
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
	}else {
		$htm = ".htm";
	}
	$file = "./$LOG_DIR/$file$htm";
	if ($CHECK[g]) { $file = "./$LOG_DIR/" . $CHECK[g] . ".htm"; }

##	$URL = eregi_replace("/$","",$URL);
	$URL = preg_replace("/\/$/i","",$URL);	

/*
	//	del ookawara 2014/04/11
	if (file_exists($file)) {
		$fp = fopen ($file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	} else {
*/
		//	商品抜き出し
		if ($CHECK[g]) {
##			$list_num = eregi_replace("^g","",$CHECK[g]);
			$list_num = preg_replace("/^g/i","",$CHECK[g]);
			$where = " AND d.list_num='$list_num'";
		}
/*
		//	del ookawara 2014/04/11
		$sql  = "SELECT count(*) AS count FROM list d" .
				" WHERE d.display='2'$where".
				" AND d.state!='1'".		//	2009/04/25	add ookawara
				";";
*/
		//	add ookawara 2014/04/11
		$sql  = "SELECT count(*) AS count FROM list d, goods g" .
				" WHERE d.pluid=g.pluid".
				" AND d.display='2'".
				$where.
				" AND d.state!='1'".
				";";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$max = $list[count];
		}
		if ($max > 0) {
			$sql  = "SELECT d.list_num, d.pluid, d.goods_name, d.size, d.color, d.comment, d.picture, d.class_m, d.c_link," .
					" b.maker_name, b.maker_url".
					", d.free_postage".		//	add ookawara 2016/07/26
					" FROM maker b, list d" .
					" WHERE b.maker_num=d.maker_num" .
					" AND d.display='2'$where" .
					" AND d.state!='1'".		//	2009/04/25	add ookawara
					" LIMIT 1;";
			$result = pg_query($db,$sql);
			$list = pg_fetch_array($result);
			$list_num = $list['list_num'];
			$pluid = $list['pluid'];
			$goods_name = $list[goods_name];
			if ($goods_name) { $goods_name = "　" . $goods_name; }
			$size = $list[size];
			if (!$size) { $size = "--"; }
			$color = $list[color];
			if (!$color) { $color = "--"; }
			$comment = $list[comment];
			$comment = nl2br($comment);
			if (!$comment) { $comment = "　"; }

/*
			//	表示の都合上、下部へ移動	del ookawara 2016/07/28
			$picture = $list[picture];
			$pic_url = "/pic/$picture";
			if ($picture && file_exists(".$pic_url")) {
				$plist = @getimagesize(".$pic_url");
				$wid = $plist[0];
				$hig = $plist[1];
				$img_msg .= "             <IMG src=\"$pic_url\" width=\"$wid\" height=\"$hig\" border=\"0\" alt=\"No.$list_num $goods_name\">\n";
			} else {
				$img_msg .= "<BR>\n<H1><B>Now Printing!</B></H1>";
			}
*/

			$class_m = $list[class_m];
			$c_link = $list[c_link];
			if ($c_link) {
				$color .= "　<A href=\"$c_link\" target=\"_blank\">カラーサンプル</A>";
			}
			$maker_url = $list[maker_url];
			$ams = $amf = "";
			if ($maker_url) {
				$ams = "<A href=\"$maker_url\" target=\"_blank\">";
				$amf = "</A>";
			}
			$maker_name = $list[maker_name];
			if ($maker_name) { $maker_name = $ams . $maker_name . $amf; }

			//$sokunou = "<img src=\"/images/sokunou.gif\" />";	//	add ookawara 2010/12/06
			$sokunou = "<span>即納</span>";

			$sql  = "SELECT stock, price, m_price, set_flag, class_m FROM goods".
					" WHERE pluid='$pluid';";
			if ($result2 = pg_query($db,$sql)) {
				$list2 = pg_fetch_array($result2);
				//$stock = $list2[stock];	//	del ookawara 2010/02/01
				$stock = $list2[stock] - $CART[$list_num];	//	add ookawara 2010/02/01
				$stock_msg = "";
				if ($stock > 0) {
					$stock_msg = <<<WAKABA
      <SELECT class="input-inline" name="buy_num">
        <OPTION value="" selected>--</OPTION>

WAKABA;
					if ($stock >= 20) { $smax = 20; } else { $smax = $stock; }
					for($i=1; $i<=$smax; $i++) {
							$stock_msg .= "        <OPTION value='$i'>$i</OPTION>\n";
					}
					$stock_msg .= "</SELECT>\n";
					$stock_msg .= "<button class=\"btn btn-cart\" type=\"submit\">購入</button>\n";
				} else {
					$stock_msg = "品切中\n";
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
				} else {
					$price = "--";
				}

				$m_price = $list2[m_price];
				if ($m_price) {
					if ($h_tax == 1) {
						$m_price = $m_price + floor(($m_price * $tax) + 0.5);
					}
					$m_price = "<S>" . number_format($m_price) . "円</S>";
				} else {
					$m_price = "--";
				}

			} else {
				$stock_msg = "品切中\n";
				$price = "--";
				$m_price = "--";
				$sokunou = "";	//	add ookawara 2010/12/06
			}

			if ($class_m >= 500 && $class_m < 600) {
				$size_msgs = "種類1";
				$color_msgs = "種類2";
			} else {
				$size_msgs = "サイズ";
				$color_msgs = "カラー";
			}

			//	add ookawara 2016/07/26
			$free_postage = $list['free_postage'];
			$img_msg = "";
			if ($souryoufree == 1 && ($free_postage > 0 || ($souryoufreeprice > 0 && $souryoufreeprice <= $check_price))) {
				$img_msg = "<div  class=\"blinking\" color:red;font-weight:bold;font-size:18px;\">基本送料無料</div>\n";	//	add ohkawara 2020/04/23	送料無料→基本送料無料に変更
			}
			$picture = $list[picture];
			$pic_url = "/pic/$picture";
			if ($picture && file_exists(".$pic_url")) {
				$plist = @getimagesize(".$pic_url");
				$wid = $plist[0];
				$hig = $plist[1];
				$img_msg .= "<IMG src=\"$pic_url\" width=\"$wid\" height=\"$hig\" border=\"0\" alt=\"No.$list_num $goods_name\">\n";
			} else {
				$img_msg .= "<BR>\n<H1><B>Now Printing!</B></H1>";
			}

			// add seo 2017-01-17
			OzzysSEO::setProductDetails(array(
				'name' => $list['goods_name'],
				'maker' => $list['maker_name'],
				'size' => $size,
				'color' => $color,
				'jancode' => $pluid,
				'comment' => $comment
			));

            // add simon 2018-07-05 >>>
            if(Souryou::prod_has_rod_fee($list2)) {
                $fee = Souryou::cart_rod_shipping_fee(array($list2));
				$fee = number_format($fee);	//	add ohkawara 2020/04/23
                $rod_fee_block = "<div class='details-block'>
				<div class='details-label stripes'>追加送料</div>
				<div class='details-data'>{$fee}円</div>
			</div>";
            }

            // <<<

			$html = <<<WAKABA
<section class="product-page">
	<FORM action="/cart.php" method="POST">
	<INPUT type="hidden" name="list_num" value="$list_num">

	<h2 class="sub-title-prod">No.$list_num $goods_name</h2>

		<div class="product-image">$img_msg</div>
		<div class="details-block">
			<div class="details-label stripes">メーカー</div>
			<div class="details-data">$maker_name</div>
		</div>

		<!-- vvv -->
		<div class="details-block">
			<div class="details-block-line">
				<div class="details-label stripes">定価</div>
				<div class="details-data"><i class="price-teika">$m_price</i></div>

				<div class="details-label stripes">値段</div>
				<div class="details-data">$price</div>
			</div>

			<div class="details-block-line">
				<div class="details-label stripes">$size_msgs</div>
				<div class="details-data">$size</div>

				<div class="details-label stripes">$color_msgs</div>
				<div class="details-data">$color</div>
			</div>
		</div>
		<!-- ^^^ -->

		<div class="details-block">
			<div class="details-label stripes">JANコード</div>
			<div class="details-data">$pluid</div>
		</div>

		<div class="details-block">
			<div class="details-label stripes">商品説明</div>
			<div class="details-data">
				<blockquote>
					$comment
				</blockquote>
			</div>
		</div>

		<div class="details-block">
			<div class="details-label stripes">納期</div>
			<div class="details-data product-instantdelivery">
				$sokunou
			</div>
		</div>

$rod_fee_block

		<div class="details-block">
			<div class="details-label stripes">ご購入</div>
			<div class="details-data">
				$stock_msg
			</div>
		</div>

	</FORM>
</section>

WAKABA;
		} else {
			$html = <<<WAKABA
<BR>
該当する商品はございません。<BR>
<BR>

WAKABA;
		}

/*
		//	del ookawara 2014/12/18
		if ($html && $stock < 1) {
			$OUT = fopen("$file","w");
			fwrite($OUT,$html);
			fclose($OUT);
			@chmod($file,0666);
		}
*/

	//}	//	del ookawara 2014/04/11

	return $html;

}
?>
