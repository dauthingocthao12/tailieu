
//	商品一覧カートsubmit
function set_goods_data(goods_code, goods_name, goods_kakaku) {

	if (document.getElementById("goods_code")){
		document.getElementById("goods_code").value = goods_code;
	}

	if (document.getElementById("goods_name")){
		document.getElementById("goods_name").value = goods_name;
	}

	if (document.getElementById("goods_kakaku")){
		document.getElementById("goods_kakaku").value = goods_kakaku;
	}

	if (document.getElementById("goods_size") && goods_code){
		var size_id_name = 'size_'+goods_code;
		if (document.getElementById(size_id_name)) {
			var select = document.getElementById(size_id_name);
			var options = document.getElementById(size_id_name).options;
			var goods_size = options.item(select.selectedIndex).value;
			document.getElementById("goods_size").value = goods_size;
		}
	}

	document.getElementById("set_cart").submit();

	return false;
}

//	商品一覧カートsubmit
function view_rireki(sells_num) {

	if (document.getElementById("rireki_code")){
		document.getElementById("rireki_code").value = sells_num;
	}

	document.getElementById("rireki_view").submit();

	return false;
}

//	フォームリセット
function form_reset(){

	if (document.getElementById("reset_email")){
		document.getElementById("reset_email").value = "" ;
	}

	if (document.getElementById("reset_sells_num")){
		document.getElementById("reset_sells_num").value = "";
	}

	if (document.getElementById("reset_pass")){
		document.getElementById("reset_pass").value = "";
	}

	if (document.getElementById("reset_checkbox")){
		document.getElementById("reset_checkbox").checked = false;
	}

}

//	二重登録防止処理
function submit_disabled(buttom){
	buttom.disabled = "true";
	buttom.form.submit();
}

//	買い物かごフォーム制御
//	add ookawara 2013/11/15
function cart_form_submit(mode, action) {	//	add action ookawara 2014/01/06

	//	mode をセットする
	if (mode && document.getElementById("cart_mode")){	//	add mode && ookawara 2014/01/06
		document.getElementById("cart_mode").value = mode;
	}

	//	action をセットする
	//	add ookawara 2014/01/06
	if (action && document.getElementById("cart_action")){
		document.getElementById("cart_action").value = action;
	}

	//	フォームボタンを使えなくする（2重送信防止）
	if (document.getElementById("cart_mode_retutrn")){
		document.getElementById("cart_mode_retutrn").disabled = "true";
	}
	if (document.getElementById("cart_mode_send")){
		document.getElementById("cart_mode_send").disabled = "true";
	}
	if (document.getElementById("cart_mode_check")){
		document.getElementById("cart_mode_check").disabled = "true";
	}
	if (document.getElementById("cart_mode_paypal")){
		document.getElementById("cart_mode_paypal").disabled = "true";
	}
	if (document.getElementById("cart_mode_modoru")){
		document.getElementById("cart_mode_modoru").disabled = "true";
	}

	if (document.getElementById("cart_form")){
		document.getElementById("cart_form").submit();
	}

	return false;
}

//	アフィリエイトポイント変換ページ
//	「変換種類」ラジオボタン表示切りかえ
function hyouji(lId) {
	if (lId == 'off') {
		msg1.style.display = "block";
	} else {
		msg1.style.display = "none";
	}
}

