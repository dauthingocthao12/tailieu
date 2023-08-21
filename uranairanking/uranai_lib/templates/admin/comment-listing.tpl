{extends file="main.tpl"}
{block name=body}

<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
<script>
$(document).ready(function() {

	var filter_site = "{$smarty.session['admin-comment-filter-site']}";
	if(filter_site) {
		$('[name=filter_site]').val(filter_site);
	}
	// change site filter
	$('[name=filter_site]').change(function() {
		$(this).parents('form').first().submit();
	});

	var filter_user = "{$smarty.session['admin-comment-filter-user']}";
	if(filter_user) {
		$('[name=filter_user]').val(filter_user);
	}
	// change user filter
	$('[name=filter_user]').change(function() {
		$(this).parents('form').first().submit();
	});

	// 自動公開ボタンのアクションの確認
	$('.buttons.button-publish').submit(function() {
		return confirm('公開するときに、自動的に（ユーザの設定によって）、お知らせメールを送ります。\nよろしいでしょうか？');
	});

});
</script>

<div class="clear">&nbsp;</div>

<div class="well">
	絞込：&nbsp;
	
	<form class="button" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="filter_status" value="reported">
		<input type="hidden" name="page" value="1">
		<button type="submit" class="btn btn-xs btn-danger"><i class="fa fa-flag"></i> 新違反報告</button>
	</form>

	<span class="admin-sep-vert"></span>

	<form class="button" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="filter_status" value="pending">
		<input type="hidden" name="page" value="1">
		<button type="submit" class="btn btn-xs btn-danger"><i class="fa fa-search"></i> {"pending"|commentStatusJapanese}</button>
	</form>

	<form class="button" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="filter_status" value="rejected">
		<input type="hidden" name="page" value="1">
		<button type="submit" class="btn btn-xs btn-warning"><i class="fa fa-ban"></i> {"rejected"|commentStatusJapanese}</button>
	</form>

	<form class="button" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="filter_status" value="published">
		<input type="hidden" name="page" value="1">
		<button type="submit" class="btn btn-xs btn-success"><i class="fa fa-check"></i> {"published"|commentStatusJapanese}</button>
	</form>

	<form class="button" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="filter_status" value="all">
		<input type="hidden" name="page" value="1">
		<input type="submit" value="全て" class="btn btn-xs btn-default">
	</form>

	<form class="button form-inline" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="page" value="1">
		<select name="filter_site" class="form-control">
			<option value="0">サイト</option>
			{foreach $sites as $id => $name}
			<option value="{$id}">{$name}</option>
			{/foreach}
		</select>
	</form>

	<form class="button form-inline" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="listing">
		<input type="hidden" name="page" value="1">
		<select name="filter_user" class="form-control">
			<option value="0">ユーザ</option>
			{foreach $users as $id => $name}
			<option value="{$id}">{$name}</option>
			{/foreach}
		</select>
	</form>

</div>
		
{if $message}
<div class="alert alert-{$message.status}">{$message.content}</div>
{/if}

<table class="table table-striped table-condensed sitecomment-listing">
	<thead>
		<tr>
			<th class="status">公開<br>状況</th>
			<th class="revision">投稿<br>状況</th>
			<th class="date">投稿日<br>審査決定日</th>
			<th class="site-data">サイトID<br>サイト名</th>
			<th class="user-data">ユーザーID<br>ハンドルネーム</th>
			<th class="evaluation">評価</th>
			<th class="content">コメント</th>
			<th class="buttons text-center">&nbsp;</th>
			<th class="buttons text-center">&nbsp;</th>
		</tr>
	</thead>
	<tbody>
		{foreach $data as $comment}
		<tr>
			<td class="comment-status-{$comment.status}">{$comment.status|commentStatusJapanese}</td>
			<td>{if $comment.parent_revision}変更{else}新規{/if}</td>
			<td class="date">{$comment.date_create|japanesedate}
				<br>{if $comment.admin_date}{$comment.admin_date|japanesedate}{else}なし{/if}</td>
			<td class="site-data">{$comment.site_id}<br>{$comment.site_name}</td>
			<td class="user-data">{$comment.user_id}<br>{$comment.handlename}</td>
			<td class="evaluation">{$comment.evaluation}</td>
			<td {if $reports[$comment.site_comment_id]}class="warning"{/if}>
				{if $reports[$comment.site_comment_id]}
				<p class="text-warning">
					<i class="fa fa-flag"></i> <small>違反報告を見るために、編集画面を使って下さい。</small>
				</p>
				{/if}
				{$comment.comment|nl2br}
			</td>
			<td class="buttons button-publish">
				{if $comment.status!='published'}
				{makelink mode="comment" action="publish" id="{$comment.site_comment_id}" value="有効" class="btn-fa btn btn-success"}
				{else}
				&nbsp;
				{/if}
			</td>
			<td class="buttons button-edit">
				{makelink mode="comment" action="input" id="{$comment.site_comment_id}" value="編集" class="btn-fa btn btn-primary"}
			</td>
		</tr>
		{/foreach}
	</tbody>
</table>


<!-- Pagination! >>.  -->
<script src='/user/js/paginator.js'></script>
{insert paginator page_name='admin-comments'}
<form method='post' id='form-paginator'>
	<input type='hidden' name='mode' value='comment'>
	<input type='hidden' name='action' value='listing'>
	<input type='hidden' name='page' value='1'>
</form>
<!-- <<<  -->

{/block}
