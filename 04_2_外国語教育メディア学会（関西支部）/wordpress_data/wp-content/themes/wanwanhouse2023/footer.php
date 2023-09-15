	<footer>
		<div class="footer-container">
			<!-- <ul class="footer-content-container footer-nav pc-nav">
				<li class="col-6">
					<a href="<?php echo home_url("") ?>">トップ</a>
				</li>
				<li class="col-6">
					<a href="<?php echo home_url("about") ?>">企業情報</a>
				</li>
				<li class="col-6">
					<span>サービス</span>
				</li>
				<li class="col-6">
					<a href="<?= home_url(BLOG_SLUG) ?>">ブログ</a>
				</li>
				<li class="col-6">
					<a href="<?php echo home_url("recruit") ?>">採用情報</a>
				</li>
				<li class="col-6">
					<a href="<?php echo home_url("contact") ?>">お問合せ</a>
				</li>
			</ul>
			<div class="border pc-nav"></div> -->
			<ul class="footer-content-container footer-nav">
				<li class="col-6"><a href="<?php echo home_url("") ?>">トップ</a></li>
				<li class="col-6 menu-list">
					<ul>
						<li>
							<a href="<?php echo home_url("trimming") ?>">トリミング</a>
						</li>
						<li>
							<a href="<?php echo home_url("shampoo-car") ?>">犬の移動美容室</a>
						</li>
						<li>
							<a href="<?php echo home_url("hotel") ?>">ドッグホテル</a>
						</li>
						<li>
							<a href="<?php echo home_url("pet-taxi") ?>">ペットタクシー</a>
						</li>
						<li>
							<a href="<?php echo home_url("net-shop") ?>">ネットショップ</a>
						</li>
					</ul>
				</li>
				<li class="col-6 menu-list">
					<ul>
						<li>
							<a href="<?php echo home_url("about")?>#shopinfor">企業情報</a>
						</li>
						<li>
							<a href="<?php echo home_url("about") ?>#access-map">アクセスマップ</a>
						</li>
					</ul>
				</li>
				<li class="col-6"><a href="<?php echo home_url("recruit") ?>">採用情報</a></li>
				<li class="col-6"><a href="<?php echo home_url(BLOG_SLUG) ?>">ブログ</a></li>
				<li class="col-6"><a href="<?php echo home_url("contact") ?>">お問合せ</a></li>
			</ul>

			<ul class="footer-content-container footer-privacy-policy">
				<li>
					<a href="<?php echo home_url("privacy-policy"); ?>">プライバシーポリシー</a>
				</li>
			</ul>

			<div class="footer-content-container">
				<div class="footer-icon text-light">
					<a href="https://www.facebook.com/%E3%83%AF%E3%83%B3%E3%83%AF%E3%83%B3%E3%83%8F%E3%82%A6%E3%82%B9-%E5%89%8D%E6%A9%8B-894466287315926" target="_blank" title="FACEBOOK">
						<span class="fa-stack">
							<i class="fa fa-circle fa-stack-2x"></i>
							<i class="fa fa-facebook fa-stack-1x fa-inverse"></i>
						</span>
					</a>
					<a href="https://www.instagram.com/wanwanhouse/" target="_blank" title="INSTAGRAM">
						<i class="fa fa-instagram"></i>
					</a>
				</div>
			</div>
		</div>

		<div class="footer-last-content">
			<div class="footer-content-container section-padding">
				<h2 class="logo-image">
					<a href="<?php echo get_home_url(""); ?>">
					<img src="<?php echo get_template_directory_uri(); ?>/shared/images/common/logo.png" alt="WAN WAN HOUSE">
					</a>
				</h2>
				<h3>ワンワンハウス</h3>
				<p class="increase-space">〒371-0035 群馬県前橋市岩神町<span class="break-word">3-12-19<br></span>
					営業時間 : 9:00~19:00／<span class="break-word">定休日 : 毎週木曜日<br></span>
					TEL <a href="tel:0272345400" class="tel">027-234-5400</a>
				</p>
			</div>
		</div>
	</footer>

	<?php wp_footer(); ?>

</body>
</html>
