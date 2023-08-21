{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}

{if $data.db}
<script>
function admin_sort_user_by_date(order){
	document.user_order.submit();
}
</script>
<table class="table table-striped">
	<thead>
		<tr>
			<th width="1%" class="nowrap">管理者</th>
			<th width="1%" class="nowrap">ユーザ ID</th>
			<th>メールアドレス</th>
			<th>ハンドルネーム</th>
			<th>登録日<span onclick="admin_sort_user_by_date()" style="cursor:pointer">▼</span></th>
			<th class="nowrap" width="1%">操作</th>
		</tr>
	</thead>
	<tbody>
		{foreach $data.db as $datum}
		<tr>
			<td>{if $datum.is_admin == 1}<i style="color:#47a447" class="fa fa-check" aria-hidden="true"></i>{else}--{/if}</td>
			<td>{$datum.user_id}</td>
			<td>{$datum.email}</td>
			<td>{$datum.handlename}</td>
			<td>{$datum.date_create}</td>
			<td class="nowrap">
				{makelink mode="user" action="delete" id="{$datum.user_id}" value="削除" class="btn btn-danger"}
				{* {makelink mode="user" action="input" id="{$datum.user_id}" value="編集" class="btn btn-primary"} *}
				{makelink mode="user" action="detail" id="{$datum.user_id}" value="詳細" class="btn btn-success"}
			</td>
		</tr>
		{/foreach}
	</tbody>
</table>
<form action="" name="user_order" method="POST">
	<input type="hidden" name="mode" value="user">
	<input type="hidden" name="ation" value="listing">
	<input type="hidden" name="user_sort_column" value="date_create">
	<input type="hidden" name="user_order" value="DESC">
</form>
{/if}
{/block}
