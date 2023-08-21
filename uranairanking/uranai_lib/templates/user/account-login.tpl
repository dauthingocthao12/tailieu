{extends file="main.tpl"}
{block name=seo}
<title>ログイン 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,ログイン">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。ログイン画面です。">

<!--OGP START-->
{include file="ogp.tpl" title="|マイページ" des="12星座占いサイトを独自に集計しランキングを出しています。ログイン画面です。"}
<!--OGP END--> 

{/block}

{block name=body}

<div class="login">
	<div class="page-center clearfix">
		<div class="col-sm-8 col-sm-offset-2">

			{if $loginError}
			<div class="alert alert-danger">
				メールアドレスまたはパスワードが間違っています。
			</div>
			{/if}

			{if $loginErrorActivate}
			<div class="alert alert-danger">
				メールによる、アカウントのアクティベーション(有効化)が完了していません。<br>登録したメールアドレスへ送信されたメールを確認してください。<br><br>
				<a href="{sitelink mode="account/activate-resend"}" class="alert-link">もう一度、アクティベーションメールを送信する</a>
			</div>
			{/if}

			{if $loginSuccess}
			<p><a class="btn btn-success right-space-small" href="{sitelink mode=$prev_page}"><i class="fa fa-arrow-left"></i> 元のページに戻る</a><a class="btn btn-primary" href="/"><i class="fa fa-arrow-left"></i> トップへ進む</a></p>
			<div class="alert alert-success">
				ログインしました。<br>
			</div>

			{else}

				<h2 class="tecen font-color">
					ログイン・会員登録
				</h2>


			<div class="tecen base-bg clearfix contents-space">
<p style="color: #777777;">ご登録されたメールアドレスとパスワードを入力して下さい。<br/>
初めてご利用の方は、右下の [新規登録のご案内へ] よりお進みください。
</p>
			<div class="col-sm-6 col-sm-offset-3">
				<form action="" method="post" class="form-inline">
					<div class="form-group">
						<label class="sr-only" for="email">メールアドレス</label>
							<div class="input-group smmargin-h">
								<span class="input-group-addon fontw-backb">
										<i class="fa fa-envelope" aria-hidden="true"></i>
								</span>
								<input class="form-control" id="email" name="email" type="text" value="{$loginemail}" placeholder="メールアドレス">
							</div>
					</div>
					<div class="form-group">
						<label class="sr-only" for="password">パスワード</label>
							<div class="input-group">
								<span class="input-group-addon fontw-backb"><i class="fa fa-key" aria-hidden="true"></i></span>
								<input class="form-control" id="password" name="password" type="password" value="" placeholder="パスワード">
							</div>
					</div>
					<div class="text-center"><input type="submit" class="btn fontw-backb smmargin-h" value="ログイン"></div>
				</form>
			</div>
				<div class="col-xs-12">
					<a href="{sitelink mode="account/intro"}" class="btn btn-danger btn-wrap">新規登録のご案内へ</a><br><br>
					<div class="row text-center">{insert ad_group id="1"}</div>
					<p class="small"><a href="{sitelink mode="account/password-lost"}">パスワードを忘れた方はコチラへ</a></p>
				</div>


			</div> <!--well-->
			{/if}

		</div>
	</div>
</div>
{/block}

