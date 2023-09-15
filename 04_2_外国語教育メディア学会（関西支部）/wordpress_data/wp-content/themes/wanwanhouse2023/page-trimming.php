<?php
/*
    グルーミング ページ用のテンプレート
*/

get_header();
?>

<section>
	<div class="top-header">
		<div class="header-content">
			<div class="top-left rounded-right trimming-top">
				<div class=" top-header-container content-container-md">
					<h2 class="top-header-title"><strong>トリミング</strong></h2>
				</div>
			</div>
			<?php get_template_part("shared/parts/header", "time-information") ?>
		</div>
	</div>
</section>

<main class="trimming">
	<section class="section-lb-pink trimming-top">
		<div class="content-container-md section-padding">
			<div class="content-box">
				当店は安心してお預かりできるスペースを設けてゆったり休憩をとりながらの滞在型トリミングを推進しております。
				わんちゃんの犬種、年齢、性格やカットの内容トリミングやシャンプーが好きか嫌いかなどそのこによってトリミングにかかる時間や休憩時間が変わってきます。<br>
				もしお急ぎでなければなるべく時間にゆとりがある日にご予約下さい。<br>
				お迎え時間にご希望がある場合はなるべく沿う形でご予約を取らせていただくのでご相談ください。
			</div>

			<div class="sep-25"></div>

			<div class="content-box line-colored text-center main-text">
				ご来店時にお待たせしないため、ご予約お願いします。
				<div class="sep-25"></div>
				<ul>
					<li>
						営業時間 : 9:00～19:00
					</li>
					<li>
						定休日 : 毎週木曜日
					</li>
					<li>
						電話番号 : <a href="tel:0272345400" class="text-decorationed">TEL 027-234-5400</a>
					</li>
				</ul>
			</div>
			<!-- Instagram投稿の部品が入る -->
			<?= get_template_part("shared/parts/common", "instagram-subpages") ?>
		</div>
	</section>

	<section class="section-leaf-pink" style="height:auto;background-color:var(--bg-light)">
		<div class="section-padding content-container-md">
			<h2>ご予約からお迎えまでの流れ</h2>
			<div class="sep-50"></div>
			<dl class="trimming-flow">
				<div class="content-box noline-nocolored text-center">
					<dt>ご予約</dt>
					<dd>まずは、お電話でご予約ください。</dd>
				</div>
				<div class="triangle text-center"></div>
				<div class="content-box noline-nocolored text-center">
					<dt>ご来店</dt>
					<dd>ご来店時にお打ち合わせさせていただきます。<br>
						はじめてのご来店の場合はカルテに詳細を記載していただきます。<br>
						「1 年以内の狂犬病予防を受けたことがわかるもの」<br>
						「首輪とリード」<br>
						「いつも食べているおやつ」をお持ちください。
					</dd>
				</div>
				<div class="triangle text-center"></div>
				<div class="content-box noline-nocolored text-center">
					<dt>お迎え</dt>
					<dd>トリミング終了時に、ご連絡させていただきます。<br>
						お迎え時に料金のお支払いをお願いいたします。
					</dd>
				</div>
			</dl>

			<div class="read-more font-serif">
				<a href="#qa">トリミングに関するよくある質問－〉</a>
			</div>
		</div>
	</section>

	<section class="section-lb-rt-pink">
		<div class="section-padding content-container-md" id="course">
			<h2>コースメニュー</h2>
			<div class="sep-50"></div>
			<div class="cource-menu">
				<div class="course-box">
					<div class="content-head center border">
						<div class="fs-1">〈コース名 〉</div>
						<div class="font-serif fs-2">シャンプーコース</div>
					</div>
					<div class="sep-10"></div>
					<div class="center fs-1">〈 施術内容 〉</div>
					<div class="sep-10"></div>

					<ul>
						<li class="menu-entry">
							・足裏バリカン
							<a href="<?= home_url("trimming-menu/foot_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・足まわりカット
							<a href="<?= home_url("trimming-menu/suspension_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・爪切り爪やすり
							<a href="<?= home_url("trimming-menu/nail_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・耳掃除
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
						<li class="menu-entry">
							・肛門腺絞り
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
						<li class="menu-entry">
							・肛門周りカット
							<a href="<?= home_url("trimming-menu/anal_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・全身シャンプー＆ブロー
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
					</ul>
				</div>

				<div class="course-box">
					<div class="content-head center border">
						<div class="fs-1">〈コース名 〉</div>
						<div class="font-serif fs-2">カットコース</div>
					</div>
					<div class="sep-10"></div>
					<div class="center fs-1">〈 施術内容 〉</div>
					<div class="sep-10"></div>

					<ul>
					<li class="menu-entry">
							・足裏バリカン
							<a href="<?= home_url("trimming-menu/foot_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・足まわりカット
							<a href="<?= home_url("trimming-menu/suspension_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・爪切り爪やすり
							<a href="<?= home_url("trimming-menu/nail_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・耳掃除
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
						<li class="menu-entry">
							・肛門腺絞り
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
						<li class="menu-entry">
							・全身シャンプー＆ブロー
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
						<li class="menu-entry">
							・全身カット
							<!-- <a href="<?= home_url("trimming-menu/") ?>">詳細 ＞</a> -->
						</li>
					</ul>
				</div>
			</div>
			<div class="sep-50"></div>

			<div class="option-content noline-colored">
				<div class="content-head center border">
					<div class="center font-sans-serif">オプションメニュー</div>
				</div>
				<ul class="option-menu">
					<div class="">
						<li class="menu-entry">
							・足裏バリカン
							<a href="<?= home_url("trimming-menu/foot_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・爪きり爪やすり
							<a href="<?= home_url("trimming-menu/nail_clippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・足まわりカット
							<a href="<?= home_url("trimming-menu/suspension_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・耳ふき
							<a href="<?= home_url("trimming-menu/ear_wipes") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・耳毛切り
							<a href="<?= home_url("trimming-menu/ear_hair_clipper") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・毛玉とり (10 分 500 円 ~)
							<a href="<?= home_url("trimming-menu/pill_removal") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・おしりカット
							<a href="<?= home_url("trimming-menu/ass_cut") ?>">詳細 ＞</a>
						</li>
					</div>

					<div class="">
						<li class="menu-entry">
							・肛門腺チェック
							<a href="<?= home_url("trimming-menu/anal_gland_check") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・歯みがき
							<a href="<?= home_url("trimming-menu/toothpaste") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・肛門まわりカット
							<a href="<?= home_url("trimming-menu/anal_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・おなかバリカン
							<a href="<?= home_url("trimming-menu/tummyclippers") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・顔カット (1 カ所 500 円 ~)
							<a href="<?= home_url("trimming-menu/face_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・ひげカット
							<a href="<?= home_url("trimming-menu/beard_cut") ?>">詳細 ＞</a>
						</li>
						<li class="menu-entry">
							・足バリカン
							<a href="<?= home_url("trimming-menu/foot_clipper") ?>">詳細 ＞</a>
						</li>
					</div>
				</ul>
			</div>
			<div class="sep-50"></div>

			<div class="center">
				<img class="image-responsive" src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_course.jpg" stylealt="トリミングコース">
			</div>
		</div>
	</section>

	<section style="background-color:var(--bg-light)">
		<div class="section-padding content-container-md">
			<h2>料金表</h2>
			<div class=sep-50></div>

			<table class="table-trimming-course">
				<thead>
					<tr>
						<th>犬の種類</th>
						<th>ベーシックコース</th>
						<th>シャンプーコース</th>
						<th>カットコース</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<th>チワワ（スムース）</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥3,000~</td>
						<td class="text-right" title="カットコース">&nbsp;</td>
					</tr>
					<tr>
						<th>チワワ ( ロング )</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥3,500~</td>
						<td class="text-right" title="カットコース">¥5,000~</td>
					</tr>
					<tr>
						<th>M・ダックス ( スムース )</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥3,500~</td>
						<td class="text-right" title="カットコース">&nbsp;</td>
					</tr>
					<tr>
						<th>M・ダックス ( ロング )</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,000~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>マルチーズ</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,000~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>シーズー</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,000~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>ヨークシャ・テリア</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,000~</td>
						<td class="text-right" title="カットコース">¥5,500~</td>
					</tr>
					<tr>
						<th>パピヨン</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,500~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>ポメラニアン</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,500~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>キャバリア</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,500~</td>
						<td class="text-right" title="カットコース">¥6,000~</td>
					</tr>
					<tr>
						<th>トイ・プードル</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,000~</td>
						<td class="text-right" title="カットコース">¥6,500~</td>
					</tr>
					<tr>
						<th>M・シュナウザー</th>
						<td class="text-right" title="ベーシックコース">¥1,500~</td>
						<td class="text-right" title="シャンプーコース">¥4,500~</td>
						<td class="text-right" title="カットコース">¥6,500~</td>
					</tr>
					<tr>
						<th>柴犬 </th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥5,800~</td>
						<td class="text-right" title="カットコース">¥7,800~</td>
					</tr>
					<tr>
						<th>コーギー</th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥6,000~</td>
						<td class="text-right" title="カットコース">¥8,000~</td>
					</tr>
					<tr>
						<th>コッカー</th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥6,800~</td>
						<td class="text-right" title="カットコース">¥9,800~</td>
					</tr>
					<tr>
						<th>シェルティ</th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥6,800~</td>
						<td class="text-right" title="カットコース">¥9,800~</td>
					</tr>
					<tr>
						<th>ボーダーコリー</th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥6,800~</td>
						<td class="text-right" title="カットコース">¥9,800~</td>
					</tr>
					<tr>
						<th>ラブラドール </th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥9,000~</td>
						<td class="text-right" title="カットコース">¥15,000~</td>
					</tr>
					<tr>
						<th>ゴールデン・レトリバー</th>
						<td class="text-right" title="ベーシックコース">&nbsp;</td>
						<td class="text-right" title="シャンプーコース">¥10,000~</td>
						<td class="text-right" title="カットコース">¥15,000~</td>
					</tr>
				</tbody>
			</table>


			<div class="sep-50"></div>

			<div class="option-price">
				<div class="bothsides-line text-center">
					シニア料金
				</div>
				<div class="sep-25"></div>

				<div class="senior">
					<div class="senior-price">
						<div class="price-title">
							<div class="text-center">シニア料金(10 歳以上)</div>
						</div>
						<div class="price-content">
							<div class="dog-type">小型犬</div>
							<div class="price-width">+ ¥1,000</div>
						</div>
						<div class="price-content">
							<div class="dog-type">中型犬</div>
							<div>+ ¥1,500</div>
						</div>
						<div class="price-content">
							<div class="dog-type">大型犬</div>
							<div>+ ¥2,000</div>
						</div>
					</div>
					<div class="senior-price">
						<div class="text-secondary">
							トリミング料金 + シニア料金（すべて税別）
						</div>
						<div class="text-left">
							2018 年 4 月 1 日より、<span class="text-red">10 歳以上のシニア犬で当店のトリミング
								を初めてご利用の方</span>は追加料金をいただきます。<br>
							10 歳以上のシニア犬で当店をご利用していただいた事のある方
							でも<span class="text-red"> 1 年以上ご利用のない場合</span>、<span class="underline underline-green fz-green-underline">ベーシックコース</span>や<span class="underline underline-green fz-green-underline">オプションメニュー</span>のみご利用頂いていたわんちゃんもシニア料金の対象となります。
						</div>
					</div>
				</div>
			</div>

			<div class="option-price ">
				<div class="sep-25"></div>
				<span class="bothsides-line text-center">
					オプション料金
				</span>
				<div class="sep-25"></div>
				<div class="senior-price">
					<div class="price-title">
						<div class="text-center">オプションメニュー(1カ所)</div>
					</div>
					<div class="price-content">
						<div class="dog-type">小型犬</div>
						<div>+ ¥500</div>
					</div>
					<div class="price-content">
						<div class="dog-type">中型犬</div>
						<div>+ ¥800</div>
					</div>
					<div class="price-content">
						<div class="dog-type">大型犬</div>
						<div>+ ¥1,000</div>
					</div>
				</div>
			</div>

			<div class="sep-25"></div>

			<div class="">
				<ul class="points-list">
					<li>
						トリミングのお預かりは 9 時から出来ます。
					</li>
					<li>
						カット等仕上がりにご納得いただけない場合、 再度お直し致します。
					</li>
					<li>
						ワンちゃんのサイズ、毛質の状態 ( 毛玉など ) により料金が変わります。
					</li>
					<li>
						ワンちゃんの年齢が 10 歳以上で、 初めて当店のトリミングをご利用の場合は「シニア料金」 がプラスされます。
					</li>
					<li>
						1 人でのトリミングが困難な場合に保定料金が発生する場合があります。
					</li>
					<li>
						ベーシックコースは小型犬のみ対応です。
					</li>
				</ul>
				<p class="text-bold">
					※表示価格はすべて税別です。
				</p>
			</div>
		</div>

		<div class="border-brown"></div>

		<div class="section-padding content-container-md">
			<div class="sep-25"></div>
			<h2>送迎サービス</h2>
			<div class="sep-50"></div>
			<div class="center">
				<img class="image-responsive" src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_sougei.jpg" alt="送迎サービスの様子">
			</div>
			<div class="sep-50"></div>
			<div class="center">
				<p class="text-left">
					以下の対象地域にお住いの方で、 ご希望のお客様には送迎サービスを行っております。 ぜひご利用ください !<br>
					※詳しくはお電話にてお問い合わせください。
				</p>
			</div>
			<div class="noline-nocolored center">
				送迎料金　片道 ¥500～
			</div>
			<div class="sep-25"></div>
			<div class="center">
				ご予約・お問合せ　TEL <a href="tel:0272345400">027-234-5400</a>
			</div>
			<div class="sep-50"></div>

			<table class="table-pickup">
				<thead>
					<tr>
						<th colspan="2">送迎対象地域</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<th>あ</th>
						<td>
							荒牧町、大渡町、大友町、石倉町、大手町、岩神町、表町
						</td>
					</tr>
					<tr>
						<th>か</th>
						<td>
							川原町、上小出町、北代田町、紅雲町、上細井町、幸塚町
						</td>
					</tr>
					<tr>
						<th>さ</th>
						<td>
							総社町、総社町高井、 総社町桜ケ丘、 新前橋町、 下小出町、 下石倉町、 昭和町、 敷島町、 下細井町、 城南町、 総社町植野、住吉町
						</td>
					</tr>
					<tr>
						<th>た</th>
						<td>
							高井町、 問屋町、千代田町							</td>
					</tr>
					<tr>
						<th>な</th>
						<td>
							南橘町、日輪時町
						</td>
					</tr>
					<tr>
						<th>は</th>
						<td>
							日吉町、本町、 平和町
						</td>
					</tr>
					<tr>
						<th>ま</th>
						<td>
						緑ヶ丘町、 三河町、 南町							</td>
					</tr>
					<tr>
						<th>ら</th>
						<td>
						龍蔵寺町							
						</td>
					</tr>
					<tr>
						<th>わ</th>
						<td>
						若宮町
						</td>
					</tr>
				</tbody>
			</table>

			<p>
				送迎対象外の場合でも、シャンプーカーでの出張サービスが可能な地域がございます。
				詳しくは<a href="<?= home_url("shampoo-car") ?>#shampoo-car" ?>こちら</a>をご確認ください。
			</p>
		</div>
	</section>

	<a name="menu"></a>
	<!-- <section class="section-rt-pink">
		<div class="section-padding content-container-md">
			<div class="center">
				<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/title_price.png" alt="price">
				<h2>メニュー内容詳細</h2>
			</div>
			<div class="sep-50"></div>
			<div class="menu-detail">
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/foot_clippers") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_asiura_barikan.jpg" alt="足裏バリカン">
						</div>
						<div class="center">
							足裏バリカン
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/nail_clippers") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_tumekiritumeyasuri.jpg" alt="爪切り爪やすり">
						</div>
						<div class="center">
							<a href="">爪切り爪やすり</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/suspension_cut") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_asi_cut.jpg" alt="足まわりカット">
						</div>
						<div class="center">
							<a href="">足まわりカット</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/ear_wipes") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_mimikaki.jpg" alt="耳ふき">
						</div>
						<div class="center">
							<a href="">耳ふき</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/ear_hair_clipper") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_mimigekiri.jpg" alt="耳毛切り">
						</div>
						<div class="center">
							<a href="">耳毛切り</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/anal_gland_check") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_koumonnsen_check.jpg" alt="肛門腺チェック">
						</div>
						<div class="center">
							<a href="">肛門腺チェック</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/anal_cut") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_koumonmawari_cut.jpg" alt="肛門まわりカット">
						</div>
						<div class="center">
							<a href="">肛門まわりカット</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/face_cut") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_kao_cut.jpg" alt="顔カット">
						</div>
						<div class="center">
							<a href="">顔カット</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/pill_removal") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_kedamatori.jpg" alt="毛玉とり">
						</div>
						<div class="center">
							<a href="">毛玉とり</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/toothpaste") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_hamigaki.jpg" alt="歯みがき">
						</div>
						<div class="center">
							<a href="">歯みがき</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/tummyclippers") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_onaka_barikan.jpg" alt="おなかバリカン">
						</div>
						<div class="center">
							<a href="">おなかバリカン</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/ass_cut") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_osiri_cut.jpg" alt="おしりカット">
						</div>
						<div class="center">
							<a href="">おしりカット</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/beard_cut") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_hige_cut.jpg" alt="ひげカット">
						</div>
						<div class="center">
							<a href="">ひげカット</a>
						</div>
					</a>
				</div>
				<div class="menu-detail-list">
					<a href="<?= home_url("blog/foot_clipper") ?>">
						<div>
							<img src="<?php echo get_template_directory_uri(); ?>/shared/images/trimming/trimming_menu_asi_barikan.jpg" alt="足バリカン">
						</div>
						<div class="center">
							<a href="">足バリカン</a>
						</div>
					</a>
				</div>
			</div>
		</div>
	</section> -->

	<a name="qa"></a>
	<section class="section-padding section-rt-pink">
		<div class="content-container-md">
			<h2>よくある質問</h2>
			<div class="sep-50"></div>

			<dl class="qa-list">
				<?php
				$qa_entries = qa_list("trimming");
				if(count($qa_entries)===0) {
					echo "<p class='text-center'>情報がありません</p>";
				}
				foreach($qa_entries as $entry) {
					/** @var WP_Post $entry */
					echo '<div class="qa-group">';
						echo "<dt>{$entry->post_title}</dt>";
						echo "<dd>{$entry->post_content}";
						echo '<div class="post-thumbnail">';
							echo get_the_post_thumbnail($entry);
						echo '</div>';
					echo "</dd>";
					echo '</div>';
				}
				?>
			</dl>
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
