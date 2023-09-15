<?php
/*
    企業情報ページ用のテンプレート
*/

get_header();
?>
<section>
	<div class="top-header">
		<div class="header-content">
			<div class="top-left rounded-right top-header-about-bg">
				<div class=" top-header-container content-container-md">
					<h2 class="top-header-title"><strong>企業情報</strong></h2>
				</div>
			</div>
			<?php get_template_part("shared/parts/header", "time-information") ?>
		</div>
	</div>
</section>

<main class="about">
	<section>
		<div class="content-container-md section-padding">
			<div class="line-colored text-center">
				<p class="main-text">ドッグホテル・トリミングご利用のお客様はお越しの際に、<br>
					お電話でご予約をお願いいたします。<br>
					店舗駐車場のほかに隣接する駐車場もございます。<br>
				<p>
				<div class="sep-25"></div>
				<ul class="text-sm">
					<li>
						営業時間 : 9:00～19:00
					</li>
					<li>
						定休日 : 毎週木曜日
					</li>
					<li>
						電話番号 : <a href="tel:0272345400" class="text-color-black">TEL 027-234-5400</a>
					</li>
				</ul>
			</div>
		</div>
	</section>
	<section class="section-lb-pink map-content" id="access-map">
		<div class="content-container-md section-padding">
			<h2>アクセス</h2>
			<div class="sep-25"></div>
			<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3211.1366919390457!2d139.05436661561092!3d36.405887997494816!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x601ef34380eddbed%3A0x9504843d917cc2bc!2z44Ov44Oz44Ov44Oz44OP44Km44K5!5e0!3m2!1sja!2sjp!4v1674541348779!5m2!1sja!2sjp" 
				width="600" height="450" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade" class="map">
			</iframe>
			<p>〒371-0035 群馬県前橋市岩神町 3-12-19</p>
			<div class="sep-25"></div>
			<div class="text-center">
				<img src="<?php echo get_template_directory_uri(); ?>/shared/images/about/main-map-img.jpg" alt="map"  class="image-responsive">
			</div>
			<p>※敷地内で歩道にはみ出さないように駐車をお願いします。</p>
		</div>
	</section>

	<section class="bg-light" id="shopinfor">
		<div class="section-padding content-container-md">
			<h2>会社概要</h2>
			<dl class="shop-infor">
				<dt>会社名</dt>
				<dd>有限会社 ワンワンハウス</dd>
				<dt>創業</dt>
				<dd>1979 年</dd>
				<dt>創立</dt>
				<dd>1990 年</dd>
				<dt>代表者</dt>
				<dd>代表取締役 樺澤功生</dd>
				<dt>資本金</dt>
				<dd>3,000,000 円</dd>
				<dt>従業員数</dt>
				<dd>5 名</dd>
			</dl>

			<h3>事業内容</h3>
			<ul class="description-list">
				<li>犬専門ペットショップ経営</li>
				<li>ベット用品ネット販売</li>
				<li>犬の移動美容室 ( トリミングカー ) 運営</li>
			</ul>

			<h3>沿革</h3>
			<dl class="history-list">
				<dt>1979 年 </dt>
				<dd>前橋市北代田町にて創業</dd>
				<dt>1988 年 </dt>
				<dd>前橋市岩神町に 2 号店を開店</dd>
				<dt>1990 年</dt>
				<dd>法人化有限会社ワンワンハウス</dd>
				<dt>2003 年</dt>
				<dd>ペット用品のネット販売を開始</dd>
				<dt>2005 年</dt>
				<dd>楽天市場にてペット用品のネット販売を開始</dd>
				<dt>2011 年</dt>
				<dd>ヤフーショッピングにてベット用品のネット販売を開始</dd>
				<dt>2017 年</dt>
				<dd>犬の移動美容室（トリミングカー）を導入し運営を開始</dd>
			</dl>
		</div>
	</section>
	<?= get_template_part("shared/parts/common", "banners") ?>
</main>
<?php
get_footer();

