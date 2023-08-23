{extends file="main.tpl"}
{block name=body}

{* <input type="date" value="{$data[0].date}"> *}

{* <pre>{$data[0]|print_r}</pre> *}
{* <pre>{$data[3]|print_r}</pre> *}
{* <pre>{$data[28]|print_r}</pre> *}
<div id="batch-status-container">
	<div style="display:flex; justify-content:space-between;">
		<div class="well status-card">
			<div><span><span class="glyphicon glyphicon-ok text-success" aria-hidden="true"></span>成功</span></div>
			<div class="hi">{$success_count}<span><span class="lighter">/{$active_plugin_count}</span></span></div></div>
		<div class="well status-card">
			<div><span><span class="glyphicon glyphicon-remove text-failed" aria-hidden="true"></span>失敗</span></div>
			<div class="hi">{$fail_count}<span class="lighter">/{$active_plugin_count}</span></div>
		</div>
		<div class="well status-card">
			<div><span><span class="glyphicon glyphicon-time" aria-hidden="true"></span>未実行</span></div>
			<div class="hi">{$pending_count}<span class="lighter">/{$active_plugin_count}</span></div>
		</div>
		<div class="well status-card">
			<div>稼働数</div>
			<div class="hi">{$active_plugin_count}</div>
		</div>
		<div class="well status-card">
			<div>稼働数(総合)</div>
			<div class="hi">{$active_plugin_count_sougo}</div>
		</div>
		<div class="well status-card">
			<div>稼働数(トピック)</div>
			<div class="hi">{$active_plugin_count_topic}</div>
		</div>
	</div>
	<table class="table table-bordered table-condensed" style="table-layout:fixed; font-size:12px;">
		<thead>
			<tr>
				<th width="10%" rowspan=3>日付</th>
				<th width="5%" rowspan=3>サイトID</th>
				<th width="20%" rowspan=3>サイト名</th>
				<th width="10%" rowspan=3>取得時間</th>
				<th width="50%" colspan=4>データ状況</th>
				<th width="10%" rowspan=3>ステータス</th>
			</tr>
			<tr>
				<th colspan=2>総合運</th>
				<th colspan=2>トピック運</th>
			</tr>
			<tr>
				<th>取得</th>
				<th>log</th>
				<th>取得</th>
				<th>topic_log</th>
			</tr>
		</thead>
		{foreach $data as $item}
		<tr class="{if !$item.is_active}off{/if}">
			<td>{$item.date}</td>
			<td>{$item.site_id}</td>
			<td>{$item.site_name}</td>
			<td>{$item.site_get_time}</td>
			{* 総合運 *}
			<td>{if $item.is_execute}<span class="label label-success">[ON]</span>{else}<span class="label label-default">[OFF]</span>{/if}</td>
			<td>{if $item.log_exists}あり{else}なし{/if}</td>
			{* トピック運 *}
			<td>{if $item.site_topic}<span class="label label-success">[ON]</span>{else}<span class="label label-default">[OFF]</span>{/if}</td>
			<td>{if $item.topic_log_exists}あり{else}なし{/if}</td>
			<td>
				{* 稼働しているもの *}
				{if $item.is_active}
					{* バッチが実行済み *}
					{if $item.batch_done}
						{* 成功 *}
						{if $item.is_success}
							<span><span class="glyphicon glyphicon-ok text-success" aria-hidden="true"></span>成功</span>
						{* 失敗 *}
						{else}
							<span><span class="glyphicon glyphicon-remove text-failed" aria-hidden="true"></span>失敗</span>
						{/if}
					{else}
					{* 未実行 *}
						<span><span class="glyphicon glyphicon-time" aria-hidden="true"></span>未実行</span>
					{/if}
				{else}
				{* 稼働していないもの *}
				─
				{/if}
			</td>
		</tr>
		{/foreach}
	</table>
</div>
{/block}
