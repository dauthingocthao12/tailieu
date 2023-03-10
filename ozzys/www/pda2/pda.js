//	商品検索
//	メーカー
function submit_maker() {
	if (self.document.search_form.s_goods_name) {
		self.document.search_form.s_goods_name.selectedIndex = 0;
	}
	self.document.search_form.submit();
}

//	分類
function submit_class() {
	if (self.document.search_form.s_goods_name) {
		self.document.search_form.s_goods_name.selectedIndex = 0;
	}
	self.document.search_form.submit();
}
