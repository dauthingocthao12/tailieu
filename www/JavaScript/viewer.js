//	商品詳細画像変更
function ImgChange(filename) {

	if (document.getElementById("chage_img")){
		document.getElementById("chage_img").src = filename;
	}

	if (document.getElementById("chage_link")){
		document.getElementById("chage_link").href = filename;
	}

}
