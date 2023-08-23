/**
 * +,-記号切り替え
 *
 * @param {jQuery} obj 開閉する要素
 */
function toggleSign(obj){
	if(obj.hasClass('content-closed')){
		obj.removeClass('content-closed').addClass('content-opened');
 }else{
		obj.removeClass('content-opened').addClass('content-closed');
	}
}

/**
 * 辛口アドバイス展開
 *
 */
$('#karakuchi').click( function(){
	//一度も通信していない場合はリクエストを飛ばす
	if( $('#karakuchi-content').children().length == 0) {
		//モーダル表示
		$('#confirm').modal('show').one('click', '#confirm-yes', function(e) {
		$('.label').remove();
			$('#confirm').modal('hide');
			karakuchiRequest();
			toggleSign($('#karakuchi'));
		});
	//既に通信済みの場合コンテンツの開閉のみ行う
	}else{
		$('#karakuchi-content').toggle();
		toggleSign($(this));
	}
});

/**
 * ajax 辛口アドバイス全体取得
 *
 */
function karakuchiRequest() {
	$('label').remove();
	$("#karakuchi").append("<span class='label label-color'>読み込み中...</span>");
	var data= {
							'action' : 'karakuchi',
							'date' :$('#karakuchi-container').data('date'),
							'data_type' : $('#karakuchi-container').data('data_type'),
							'star':$('#karakuchi-container').data('star')
						};

	$.ajax({
		type: 'GET',
		url: '12star_app_ajax.php',
		dataType: 'html',
		data: data,
		success : function(data) {
			$('.label').remove();
			$('#karakuchi-content').html(data);
		}
	});
}

/**
 * ajax 順位別サイト一覧取得
 *
 */
$('#karakuchi-container').on('click','div.karakuchi-rank', function() {
	var self = $(this);
	//一度も通信していない場合ajaxでデータ取得
	if(self.next('.sites').length == 0) {
		$('.label').remove();
		self.append("<span class='label label-color'>読み込み中...</span>");
		var data= {
								 'action' : 'karakuchi-rank',
								 'rank' : self.data('rank'),
								 'date' : self.data('date'),
								 'data_type' : $('#karakuchi-container').data('data_type'),
								 'star':$('#karakuchi-container').data('star')
							 };

		$.ajax({
			type: 'GET',
			url: '12star_app_ajax.php',
			dataType: 'html',
			data: data,
			success : function(data) {
				self.siblings('.karakuchi-rank').next('.sites').hide();
				self.after(data);
				toggleSign(self);
				$('.label').remove();
				self.siblings('.karakuchi-rank').removeClass('content-opened').addClass('content-closed');
			}	
		});
	//既に通信済みの場合開閉のみ行う
	}else{
		self.next().toggle();
		toggleSign(self);
		self.siblings('.karakuchi-rank').next('.sites').hide();
		self.siblings('.karakuchi-rank').removeClass('content-opened').addClass('content-closed');
	}
});

/**
 * 日付遷移
 *
 */
$('#karakuchi-container').on('click','#d_prev, #d_today, #d_next', function() {
	$('.label').remove();
	$(this).after("<span class='label label-color date-loading'>読み込み中</span>");
	var data = {
							'action' : 'move-date',
							'date' : $(this).data('date'),
							'data_type' : $('#karakuchi-container').data('data_type'),
							'star':$('#karakuchi-container').data('star')
						};

	$.ajax({
		type: 'GET',
		url: '12star_app_ajax.php',
		dataType: 'html',
		data: data,
		success : function(data) {
			$('.label').remove();
			$('#karakuchi-content').html(data);
		}
	});
});

/**
 * グラフをタップしたら初めて表示する
 *
 */
$('#karakuchi-container').on('click','.ct-chart-line, .graph-wrapper', function(){
	$('.graph-wrapper .graph-layer').hide();
	$('.ct-chart-line .ct-series').show();
	$('#karakuchi-content').addClass('graph-clicked');
});

/**
 * "設定"モーダル表示トグル
 *
 */
$('.configuration').click( function(e){
	e.stopPropagation();	
	e.preventDefault();
	$('.config-modal').toggle();
});

/**
 * "設定"モーダル表示中に範囲外をクリックしたらモーダルを再び隠す
 *
 */
$(document).click( function (){
	if($('.config-modal').css('display') == 'block'){
		$('.config-modal').hide();
	}
});

/**
 * ユーザーテーマ変更
 * モーダル内でテーマ名をクリックしたらそのテーマに変更する
 */
$('.config-modal a').click( function(e){
	e.preventDefault();
	var theme = $(this).attr('class').replace('theme-','');//テーマ名aタグのclassには"theme-{テーマ名}"と命名すること
	var lastClass = $('.body').attr('class').split(' ')[1];//
	$('.body').removeClass(lastClass).addClass(theme);
	localStorage.setItem("user-theme", theme);
});

/**
 * 設定モーダル テーマ名の選択をハイライトする
 *
 */
$('.configuration').click( function() {
	var userTheme = localStorage.getItem('user-theme');
	if(userTheme){
		$('.config-modal li').removeClass('nav-selected');
		$('.config-modal').find(".theme-" + userTheme).parent('li').addClass('nav-selected');
	}else{
		$('.config-modal .theme-main').parent('li').addClass('nav-selected');
	}
});
