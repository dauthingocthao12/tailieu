{extends file="main.tpl"}
{block name=seo}
<title>コメント機能の使い方 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,コメント機能の使い方">
<meta name="description" content="各種占いサイトに対するレビューをかけるコメント機能の使い方ページです。">
{/block}

{block name=body}
<div class="comment-howtouse">

	<div class="title row tecen">
		<h2 class="font-color">コメント機能の使い方</h2>
	</div>

	<div class="col-xs-12 base-bg contents-space">
		<ul class="internal-link-format clearfix">
			<li><a href="#post-comment">投稿のやり方</a></li>
			<li><a href="#edit-conf">投稿の編集・確認のやり方</a></li>
			<li><a href="#icon">アイコン画像の変更</a></li>
		</ul>
		<p>説明不要な方はこちら</p>
		<div class="tecen font-bigger">
			<a href="/site-list/#site-lists" class="button pd-20 vt-spacer-small">今すぐ評価する!</a>
		</div>
		<h3 id="post-comment">投稿のやり方</h3>
		<section>
			<dl>
			<dt>①12星座占いランキングのマイページにログインしてください。<br>ログイン・新規会員登録(無料)を済ませていない方は<a href="{sitelink mode="account/login"}">こちら</a>。</dt>
			<dt>②サイト一覧からコメントしたいサイトをクリックします。</dt>
			<dd><img src="/user/img_re/comment-howtouse-img1.jpg" class="width-max" alt="サイト一覧ページの画像"></dd>
			<dt>③口コミを書くから、そのサイトの評価数、コメントを記入後、送信で投稿完了です♪<br>(投稿できるのは1サイトにつき1投稿です。コメントを変更したい場合は投稿を編集してください)</dt>
			<dd>色々なサイトの口コミを書いて、みんなで情報を共有しましょう♪
				<br>
				<img src="/user/img_re/comment-howtouse-img2.jpg" class="width-max" alt="投稿フォームの画像">
				<br>
				※他人の誹謗中傷・不適切な発言の表示を防止するため、皆さんに投稿していただいたコメントは、<br>一度管理人の方でチェックした後、表示させていただきます。<br>
				コメントの反映まで、1日～10日のお時間を頂いてしまいますが、ご了承ください。
			</dd>
		</section>

		<h3 id="edit-conf">投稿の編集・確認のやり方</h3>
		<section>
			<dl>
			<dt>①マイページ→コメント管理で、投稿したコメント一覧が表示されます</dt>
			<dd><img src="/user/img_re/comment-howtouse-img3.jpg" class="width-max" alt="投稿管理画面の画像"></dd>
			<dt>②図の<i class="fa fa-pencil"></i>マークをクリックで、対象サイトのコメントページに移動しますので、そこから編集してください。</dt>
			<dd>
			</dd>
			<dt>③その他機能説明</dt>
			<dd>
				<ul>
					<li>状態…現在のコメントの表示状況です。<br><i class="fa fa-eye-slash"></i>でコメントの公開・非公開を設定できます。<br>【公開中】→HPに公開されています<br>【審査中】→管理人のチェック待ちです<br>【無効】→管理人チェック後、HPに公開できない内容が含まれているため、公開されていない状況です。</li>
					<li>投稿日…投稿された日付です。</li>
					<li>サイト名…投稿した占いサイトの名前です。</li>
					<li>評価…投稿した占いサイトの評価数です。</li>
					<li>コメント…記入いただいたコメント内容です。</li>
					<li><i class="fa fa-heart" aria-hidden="true"></i>…コメントに「いいね」をされた数です。</li>
				</ul>
			</dd>
		</section>

		<h3 id="icon">アイコン画像の変更</h3>
		<section>
			<dl>
			<dt>①ログイン後マイページ→登録情報変更で現在のアイコン画像が表示されます</dt>
			<dd><img src="/user/img_re/comment-howtouse-img4.jpg" class="width-max" alt="ユーザー画像変更画面の画像"></dd>
			<dt>②　<i class="fa fa-pencil-square" aria-hidden="true"></i>をクリック後変更したい画像を選択してください</dt>
			<dd><img src="/user/img_re/comment-howtouse-img5.jpg" class="width-max" alt="ユーザー画像変更画面の画像"></dd>
			<dt>③保存で変更終了です</dt>
		</section>
		
		<section class="position-right">
			<a href="{sitelink mode="site-list"}#site-lists" class="button">実際にサイトのコメントを投稿しに行く！</a>
		</section>

	</div>


</div>
{/block}
