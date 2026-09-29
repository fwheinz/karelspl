"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode = require("vscode");
const child = require("child_process");


async function activate(context) {
    context.subscriptions.push(vscode.commands.registerCommand("oth.launchGUIBrowser", 
        () => {
            let vncurl = `https://${process.env['CODESPACE_NAME']}-6080.${process.env['GITHUB_CODESPACES_PORT_FORWARDING_DOMAIN']}/?autoconnect=true&resize=scale`
            vscode.env.openExternal(vscode.Uri.parse(vncurl))
        }
    ))
    context.subscriptions.push(vscode.commands.registerCommand("oth.launchGUIInternal", 
        async () => {
            let vncurl = `https://${process.env['CODESPACE_NAME']}-6080.${process.env['GITHUB_CODESPACES_PORT_FORWARDING_DOMAIN']}/?autoconnect=true&resize=scale`
            await vscode.commands.executeCommand('workbench.action.splitEditorRight')
            await vscode.commands.executeCommand('simpleBrowser.api.open', vncurl)
        }
    ))
    context.subscriptions.push(vscode.commands.registerCommand("oth.startProgram", 
        async () => {
            await vscode.commands.executeCommand("workbench.action.terminal.sendSequence", { text : "\x03" })
            vscode.window.activeTerminal.sendText("/workspaces/$RepositoryName/run.sh")
        }
    ))
}
exports.activate = activate
