<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!-->
<html class="no-js"> <!--<![endif]-->
	<head>

		{* 開発用GA4 start *}
		{* ※本番にアップロードしないこと *}
		<!-- Google tag (gtag.js) -->
		<script async src="https://www.googletagmanager.com/gtag/js?id=GT-PL9D5X7"></script>
		<script>
        window.dataLayer = window.dataLayer || [];
        function gtag(){dataLayer.push(arguments);}
		gtag('js', new Date());

        gtag('config', 'GT-PL9D5X7');
		</script>
		{* 開発用GA4 end *}

		{* 開発用GTM start*}
		{* ※本番にアップロードしないこと *}
		<!-- Google Tag Manager -->
		<script>(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
		new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
		j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
		'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
		})(window,document,'script','dataLayer','GTM-MXK76BM');</script>
		<!-- End Google Tag Manager -->
		{* 開発用GTM end *}

		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		{block name="seo"}
		<title>{$star_name}　今日の星座占い　良い結果だけを見たいあなたに・・・</title>
		<meta name="keywords" content="12星座占いランキング,占い,ランキング">
		<meta name="description" content="各サイトから情報を取得して12星座占いのランキングを表示しております。">
		{/block}
		<meta property="og:image" content="http://uranairanking.jp/user/img_re/new-fb-thumbnail.jpg" />
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<!-- Place favicon.ico and apple-touch-icon.png in the root directory -->
		<link rel="stylesheet" href="/user/css/normalize.css">
		<link rel="stylesheet" href="/user/css/main.css">
		<link rel="stylesheet" href="/user/packages/bootstrap-3.1.1-dist/css/bootstrap.min.css">
		<link rel="stylesheet" href="/user/packages/font-awesome-4.5.0/css/font-awesome.min.css">
		<!--app things-->
		<link rel="stylesheet" href="/user/css/12star_app.css?var={$config.cache_date}">
		<link rel="stylesheet" href="/user/css/12star_other.css?var={$config.cache_date}">
		 <!-- style override for nav.navbar etc. -->
		{literal}
		<!-- ユーザーヒート -->
		<script type="text/javascript">
		(function(add, cla){window['UserHeatTag']=cla;window[cla]=window[cla]||function(){(window[cla].q=window[cla].q||[]).push(arguments)},window[cla].l=1*new Date();var ul=document.createElement('script');var tag = document.getElementsByTagName('script')[0];ul.async=1;ul.src=add;tag.parentNode.insertBefore(ul,tag);})('//uh.nakanohito.jp/uhj2/uh.js', '_uhtracker');_uhtracker({id:'uhlHJgq9kn'});
		</script>
		<!-- ユーザーヒート終了 -->
		{/literal}
		<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
		<script src="//cdnjs.cloudflare.com/ajax/libs/jquery-cookie/1.4.1/jquery.cookie.min.js"></script>
	</head>
	<body>

		{* 開発用GTM start*}
		{* ※本番にアップロードしないこと *}
		<!-- Google Tag Manager (noscript) -->
		<noscript><iframe src="https://www.googletagmanager.com/ns.html?id=GTM-MXK76BM"
		height="0" width="0" style="display:none;visibility:hidden"></iframe></noscript>
		<!-- End Google Tag Manager (noscript) -->
		{* 開発用GTM end *}

	<div class="body">
	<script>
	/**
   * ローカルストレージにユーザー設定テーマがあれば設定する
   *
   */
	function loadUserTheme() {
		var userTheme = localStorage.getItem("user-theme");
		if(userTheme){
			$(".body").removeClass().addClass('body').addClass(userTheme);
		}else{
			$('.body').addClass('main');
		}
	}
	
	loadUserTheme(); //テーマを呼び出す
	</script>
	{block name=body}{/block}
	</div>
	</body>
	<script src="/user/js/vendor/modernizr-2.6.2.min.js?var={$config.cache_date}"></script>
	<script src="/user/js/12star_app.js?var={$config.cache_date}"></script>
	<script src="/user/packages/bootstrap-3.1.1-dist/js/bootstrap.min.js?var={$config.cache_date}"></script>
</html>
