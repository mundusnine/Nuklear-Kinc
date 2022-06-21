const project = new Project('Nuklear');

await project.addProject('../../Kinc');

project.addFile('Sources/**');
project.addFile('Shaders/**');
project.setDebugDir('Deployment');

resolve(project);
