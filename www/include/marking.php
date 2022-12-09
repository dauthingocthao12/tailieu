<?PHP

//	チームオーダー入力画面
function marking( $KAGOS, $ERROR ) {

	//	選択項目
	global $SEBAN_N,$SENAME_N,$MUNEBAN_N,$PANT_N,$BACH_N;
	//	価格
	global $SEBAN_P_N,$SENAME_P_N,$MUNEBAN_P_N,$PANT_P_N,$BACH_P_N;

	//	エラーメッセージ
	if ( $ERROR ) {
		$error_html = error_html($ERROR);

		//	エラー時に値を各フォームに反映する
		$hinban = $_SESSION['marking']['hinban'];			//	マーキング商品
		$seban_l = $_SESSION['marking']['seban_l'];			//	背番号(タイプ)
		$seban_num = $_SESSION['marking']['seban_num'];		//	背番号(番号)
		$sename_l = $_SESSION['marking']['sename_l'];		//	背ネーム(タイプ)
		$sename_name = $_SESSION['marking']['sename_name'];	//	背ネーム(番号)
		$muneban_l = $_SESSION['marking']['muneban_l'];		//	胸番号(タイプ)
		$muneban_num = $_SESSION['marking']['muneban_num'];	//	胸番号(番号)
		$pant_l = $_SESSION['marking']['pant_l'];			//	パンツ番号(タイプ)
		$pant_num = $_SESSION['marking']['pant_num'];		//	パンツ番号(番号)
		$bach_l = $_SESSION['marking']['bach_l'];			//	バッチ

	}

	//	マーキング商品
	/*
	if ( !$hinban ) {
		$selected_hinban = "selected";
	} else {
		$selected_hinban = "";
	}
	*/
	$max = count($KAGOS);

	for($i=0; $i<$max; $i++) {
##		list($hinban_,$title_,$kakaku_,$num_) = split("::",$KAGOS[$i]);
		list($hinban_,$title_,$kakaku_,$num_) = explode("::",$KAGOS[$i]);
		if ($hinban_) {
			if ($hinban == $hinban_) {
				$selected2 = "selected";
			} else {
				$selected2 = "";
			}
			$hinban_list .= "              <option value=\"$hinban_\" $selected2>$title_</option>\n";
		}
	}

	if ( $hinban == "mochikomi" ) {
		$selected_hinban2 = "selected";
	} else {
		$selected_hinban2 = "";
	}
	$hinban_list .= "              <option value=\"mochikomi\" $selected_hinban2>持ち込み商品</option>\n";

	//	背番号
	/*
	if ( !$seban_l ) {
		$selected_seban = "selected";
	} else {
		$selected_seban = "";
	}
	*/
	$max = count($SEBAN_N);

	for($i=1; $i<$max; $i++) {
		if ( $seban_l == $i ) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = floor($SEBAN_P_N[$i] * ($TAX_ + 1) + 0.5);
		$price_t = number_format($price_t);
		if ($price_t == 0) { continue; }
		$seban_list .= "              <option value=\"$i\" $selected>$SEBAN_N[$i]：$price_t 円/1文字</option>\n";
	}

	//	背ネーム
	/*
	if ( !$sename_l ) {
		$selected_seban = "selected";
	} else {
		$selected_seban = "";
	}
	*/
	$max = count($SENAME_N);
	for($i=1; $i<$max; $i++) {
		if ( $sename_l == $i ) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = floor($SENAME_P_N[$i] * ($TAX_ + 1) + 0.5);
		$price_t = number_format($price_t);
		if ($price_t == 0) { continue; }
		$sename_list .= "              <option value=\"$i\" $selected>$SENAME_N[$i]：$price_t 円/1文字</option>\n";
	}

	//	胸番号
	/*
	if ( !$muneban_l ) {
		$selected_muneban = "selected";
	} else {
		$selected_muneban = "";
	}
	*/
	$max = count($MUNEBAN_N);
	for($i=1; $i<$max; $i++) {
		if ( $muneban_l == $i ) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = floor($MUNEBAN_P_N[$i] * ($TAX_ + 1) + 0.5);
		$price_t = number_format($price_t);
		if ($price_t == 0) { continue; }
		$muneban_list .= "              <option value=\"$i\" $selected>$MUNEBAN_N[$i]：$price_t 円/1文字</option>\n";
	}

	//	パンツ番号
	/*
	if ( !$pant_l ) {
		$selected_pant = "selected";
	} else {
		$selected_pant = "";
	}
	*/
	$max = count($PANT_N);
	for($i=1; $i<$max; $i++) {
		if ($pant_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = floor($PANT_P_N[$i] * ($TAX_ + 1) + 0.5);
		$price_t = number_format($price_t);
		if ($price_t == 0) { continue; }
		$pant_list .= "              <option value=\"$i\" $selected>$PANT_N[$i]：$price_t 円/1文字</option>\n";
	}

	//	バッジ
	/*
	if (!$bach_l) {
		$selected_bach = "selected";
	} else {
		$selected_bach = "";
	}
	*/
	$max = count($BACH_N);
	for($i=1; $i<$max; $i++) {
		if ($bach_l == $i) {
			$selected = "selected";
		} else {
			$selected = "";
		}
		$price_t = floor($BACH_P_N[$i] * ($TAX_ + 1) + 0.5);
		$price_t = number_format($price_t);
		if ($price_t == 0) { continue; }
		$bach_list .= "              <option value=\"$i\" $selected>$BACH_N[$i]：$price_t 円</option>\n";
	}

	$INPUTS['HINBANLIST'] = $hinban_list;	//	マーキング商品
	$INPUTS['SEBANLIST'] = $seban_list;		//	背番号(プルダウン)
	$INPUTS['SENAMELIST'] = $sename_list;	//	背ネーム(プルダウン)
	$INPUTS['MUNEBANLIST'] = $muneban_list;	//	胸番号(プルダウン)
	$INPUTS['PANTLIST'] = $pant_list;		//	パンツ番号(プルダウン)
	$INPUTS['BACHLIST'] = $bach_list;		//	バッチ
	$INPUTS['ERRORMSG'] = $error_html;		//	エラーメッセージ

	//	エラー時に値を各フォームに反映する
	$INPUTS['SEBANNUM'] = $seban_num;		//	背番号(番号)
	$INPUTS['SENAMENAME'] = $sename_name;	//	背ネーム(番号)
	$INPUTS['MUNEBANNUM'] = $muneban_num;	//	胸番号(番号)
	$INPUTS['PANTNUM'] = $pant_num;			//	パンツ番号(番号)

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("marking.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}

//	買い物かごへリダイレクト
function marking_redirect( $KAGOS, $OPTIONS ) {

	//	SESIONに保持
	$op_num = $_SESSION['marking']['op_num'];
	$hinban = $_SESSION['marking']['hinban'];
	$title = $_SESSION['marking']['title'];
	$seban_l = $_SESSION['marking']['seban_l'];
	$seban_num = $_SESSION['marking']['seban_num'];
	$sename_l = $_SESSION['marking']['sename_l'];
	$sename_name = $_SESSION['marking']['sename_name'];
	$muneban_l = $_SESSION['marking']['muneban_l'];
	$muneban_num = $_SESSION['marking']['muneban_num'];
	$pant_l = $_SESSION['marking']['pant_l'];
	$pant_num = $_SESSION['marking']['pant_num'];
	$bach_l = $_SESSION['marking']['bach_l'];

	//	登録処理
	if ( $KAGOS ) {
		foreach ( $KAGOS AS $val ) {
##			list( $hinban_,$title_,$kakaku_,$num_ ) = split( "::",$val );
			list( $hinban_,$title_,$kakaku_,$num_ ) = explode( "::",$val );
			if ( $hinban == $hinban_ ) {
				$title = $title_;
			}
		}
	}
	if ( $hinban == "mochikomi" ) { $title = "持ち込み商品"; }

	$opt = "";
	$op_max_num = 0;
	if ( $OPTIONS ) {
		foreach ( $OPTIONS AS $VAL ) {
			//	すでに保持しているマーキング商品を保持する
			$opt .= $VAL."<>";
			list( $op_num_ ) = explode( "::",$VAL );
			if ( $op_max_num < $op_num_ ) {
				$op_max_num = $op_num_;
			}
		}
	}

	//	保持している商品に今回追加した商品を追加してSESSIONに格納する
	$op_num = $op_max_num + 1;
	$n_option = "$op_num::$hinban::$title::$seban_l::$seban_num::$sename_l::$sename_name::$muneban_l::$muneban_num::$pant_l::$pant_num::$bach_l::<>";
	$opt .= $n_option;
	$_SESSION['opt'] = $opt;

	//	SESSIONリセット
	unset( $_SESSION['marking'] );

	header ("Location: ./cago.php\n\n");
	exit();

}

//	エラーチェック
function marking_check( &$ERROR ) {

	//	POST値を取得
	$op_num = $_POST['op_num'];
	$hinban = $_POST['hinban'];
	$title = $_POST['title'];
	$seban_l = $_POST['seban_l'];
	$seban_num = $_POST['seban_num'];
	$sename_l = $_POST['sename_l'];
	$sename_name = $_POST['sename_name'];
	$muneban_l = $_POST['muneban_l'];
	$muneban_num = $_POST['muneban_num'];
	$pant_l = $_POST['pant_l'];
	$pant_num = $_POST['pant_num'];
	$bach_l = $_POST['bach_l'];

	//	コンバート処理
#	$seban_num = mb_convert_kana($seban_num,"sn","EUC-JP");
	$seban_num = mb_convert_kana($seban_num,"sn","UTF-8");
	$seban_num = str_replace(' ', '', $seban_num);
#	$sename_name = mb_convert_kana($sename_name,"sn","EUC-JP");
	$sename_name = mb_convert_kana($sename_name,"sn","UTF-8");
#	$muneban_num = mb_convert_kana($muneban_num,"sn","EUC-JP");
	$muneban_num = mb_convert_kana($muneban_num,"sn","UTF-8");
	$muneban_num = str_replace(' ', '', $muneban_num);
#	$pant_num = mb_convert_kana($pant_num,"sn","EUC-JP");
	$pant_num = mb_convert_kana($pant_num,"sn","UTF-8");
	$pant_num = str_replace(' ', '', $pant_num);

	//	SESIONに保持
	$_SESSION['marking']['op_num'] = $op_num;
	$_SESSION['marking']['hinban'] = $hinban;
	$_SESSION['marking']['title'] = $title;
	$_SESSION['marking']['seban_l'] = $seban_l;
	$_SESSION['marking']['seban_num'] = $seban_num;
	$_SESSION['marking']['sename_l'] = $sename_l;
	$_SESSION['marking']['sename_name'] = $sename_name;
	$_SESSION['marking']['muneban_l'] = $muneban_l;
	$_SESSION['marking']['muneban_num'] = $muneban_num;
	$_SESSION['marking']['pant_l'] = $pant_l;
	$_SESSION['marking']['pant_num'] = $pant_num;
	$_SESSION['marking']['bach_l'] = $bach_l;

	$ERROR = array();
	if (!$hinban) { $ERROR[] = "マーキング商品が選択されておりません。"; }
	if (!$seban_l && $seban_num) { $ERROR[] = "背番号のタイプが選択されておりません。"; }
	if ($seban_l && $seban_num == "") { $ERROR[] = "背番号の番号が入力されておりません。"; }
##	if ($seban_num && ereg("[^0-9]",$seban_num)) { $ERROR[] = "背番号の番号が不正です。"; }
	if ($seban_num && preg_match("/[^0-9]/",$seban_num)) { $ERROR[] = "背番号の番号が不正です。"; }
	if (!$sename_l && $sename_name) { $ERROR[] = "背ネームのタイプが選択されておりません。"; }
	if ($sename_l && $sename_name == "") { $ERROR[] = "背ネームのネームが入力されておりません。"; }
##	if ($sename_name && eregi("[^A-Z0-9\,-\_\. ]",$sename_name)) { $ERROR[] = "背ネームのネームが不正です。"; }
	if ($sename_name && preg_match("/[^A-Z0-9\,-\_\. ]/i",$sename_name)) { $ERROR[] = "背ネームのネームが不正です。"; }
	if (!$muneban_l && $muneban_num) { $ERROR[] = "胸番号のタイプが選択されておりません。"; }
	if ($muneban_l && $muneban_num == "") { $ERROR[] = "胸番号の番号が入力されておりません。"; }
##	if ($muneban_num && ereg("[^0-9]",$muneban_num)) { $ERROR[] = "胸番号の番号が不正です。"; }
	if ($muneban_num && preg_match("/[^0-9]/",$muneban_num)) { $ERROR[] = "胸番号の番号が不正です。"; }
	if (!$pant_l && $pant_num) { $ERROR[] = "パンツ番号のタイプが選択されておりません。"; }
	if ($pant_l && $pant_num == "") { $ERROR[] = "パンツ番号の番号が入力されておりません。"; }
##	if ($pant_num && ereg("[^0-9]",$pant_num)) { $ERROR[] = "パンツ番号の番号が不正です。"; }
	if ($pant_num && preg_match("/[^0-9]/",$pant_num)) { $ERROR[] = "パンツ番号の番号が不正です。"; }
	if ($hinban && !$seban_l && !$seban_num && !$sename_l && !$sename_name && !$muneban_l && !$muneban_num && !$pant_l && !$pant_num && !$bach_l) {
		$ERROR[] = "マーキング内容が選択されておりません。";
	}

}

?>