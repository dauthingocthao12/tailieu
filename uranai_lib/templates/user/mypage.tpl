{extends file="main.tpl"}
{block name=seo}
<title>ユーザー情報 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,ユーザー情報">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。ユーザー情報の登録、変更ページです。">
{/block}
{block name=body}


<div class="mypage container">
	<h2 class="font-color"><i class="fa fa-user" aria-hidden="true"></i>マイページ</h2>
	<div class="base-bg clearfix">
		<div class="col-sm-6 col-sm-offset-3 btn-list">
			<div class="logout-button">
				<p>
				{if !$user}
				<a href="{sitelink mode="account/login"}" class="btn btn-primary"><i class="fa fa-arrow-left"></i>戻る</a>
				{else}
				<a href="{sitelink mode="account/logout"}" class="btn btn-danger"><i class="fa fa-unlock"></i>ログアウト</a>
				{/if}
				</p>
			</div>
			<div class="list-button">
				<a href="{sitelink mode="registered-person"}" class="btn btn-primary"><i class="fa fa-list-alt" aria-hidden="true"></i>順位一覧ページを見る</a>
			</div>
			<div class="info-button">
				<a href="{sitelink mode="account/form"}" class="btn btn-success"><i class="fa fa-refresh" aria-hidden="true"></i>登録情報変更</a>
			</div>
			<div class="info-button">
				<a href="{sitelink mode="account/comment"}" class="btn btn-success">
					{insert userCommentWarning}<i class="fa fa-commenting" aria-hidden="true"></i>コメント管理</a>
			</div>
			<div class="del-button">
				<a href="{sitelink mode="account/unregist"}" class="btn btn-default"><i class="fa fa-close"></i>ユーザー削除</a>
			</div>
		
		</div>
	</div>


</div>


{/block}
