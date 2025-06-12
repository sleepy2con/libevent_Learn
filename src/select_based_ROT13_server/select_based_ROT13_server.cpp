// select_based_ROT13_server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#endif

#include <memory>
#include <assert.h>
#include <vector>

const int MAX_LINE = 16384;
const int FD_SETSIZE_1024 = 1024;

char rot13_char(char c)
{
	/* We don't want to use isalpha here; setting the locale would change
	 * which characters are considered alphabetical. */
	if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
		return c + 13;
	else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
		return c - 13;
	else
		return c;
}

struct fd_state {
	char buffer[MAX_LINE];	// 存recv的数据
	size_t buffer_used;	// 记录读的位置
	int writing;	// 标记准备写数据
	size_t n_wirtten;	// 记录当前写的位置
	size_t write_upto;	//
};

struct fd_state* alloc_fd_state(void)
{
	//struct fd_state* state = (struct fd_state*)malloc(sizeof(struct fd_state)); // C
	struct fd_state* state = new fd_state{ "",0,0,0,0 };
	if (!state)
		return nullptr;
	return state;
}
void free_fd_state(struct fd_state* state)
{
	//free(state);
	delete state;
}

std::unique_ptr<struct fd_state> alloc_fd_state_unique(void)
{
	//auto state = std::make_shared<struct fd_state>();
	auto state = std::make_unique<struct fd_state>();
	*state = { "", 0, 0, 0, 0 };
	//state->buffer_used = state->n_wirtten = state->writing = state->wirte_upto = 0;
	return state;
}

void make_nonblock(int fd)
{
	// 一般情况下，recv这样的函数会阻塞程序，如果没收到就不会继续执行，
	// 设置为非阻塞模式，recv，read accept这样的函数会立刻返回，不会等
#ifdef _WIN32
	u_long mode = 1;
	ioctlsocket(fd, FIONBIO, &mode); // 这里只是用mode的值，mode销毁也没关系
#else
	fcntl(fd, F_SETFL, O_NONBLOCK);
#endif
}

int do_read(int fd, struct fd_state* state)
{
	char buf[1024];
	int result;
	while (1)
	{
		result = recv(fd, buf, sizeof(buf), 0);
		if (result <= 0)
			break;
		for (int i = 0; i < result; ++i)
		{
			if (state->buffer_used < sizeof(state->buffer))
				state->buffer[state->buffer_used++] = rot13_char(buf[i]);
			if (buf[i] == '\n')
			{
				state->writing = 1;
				state->write_upto = state->buffer_used;
			}
		}
	}
	if (result == 0)
		return 1; // 连接关闭
	else if (result < 0)
	{
		int err = WSAGetLastError();
		//std::cerr << "recv error code: " << err << std::endl;
		//非阻塞读无数据
		if (errno == EAGAIN)
			return 0;
		if (err == 10035)
			return 0;
		return -1;		// 其他错误
	}
	return 0;	// 读取正常完成
}

int do_write(int fd, fd_state* state)
{
	while (state->n_wirtten < state->write_upto)
	{
		int result = send(fd, state->buffer + state->n_wirtten, state->write_upto - state->n_wirtten, 0);
		if (result < 0)
		{
			if (errno == EAGAIN)
				return 0;
			return -1;
		}
		assert(result != 0);
		state->n_wirtten += result;
	}
	if (state->n_wirtten == state->buffer_used)
		state->n_wirtten = state->write_upto = state->buffer_used = 0;
	state->n_wirtten = 0;
	return 0;
}

void run() {
	int listener;
	//struct fd_state* state[FD_SETSIZE];
	// windows 上select最多处理64个连接,为了处理fd > 64情况,修改下这玩意
	std::vector<std::unique_ptr<fd_state>> state(FD_SETSIZE_1024);
	struct sockaddr_in sin;
	int maxfd;
	fd_set readset, writeset, exset;

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(40713);

	for (int i = 0; i < FD_SETSIZE_1024; i++)
	{
		state[i] = nullptr;
	}
	listener = socket(AF_INET, SOCK_STREAM, 0);
	make_nonblock(listener);	// 设置非阻塞模式

	int one = 1;	// 设置端口复用
#ifdef _WIN32
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));
#else
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
#endif
	if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		std::cerr << "bind";
		return;
	}
	if (listen(listener, 16) < 0)
	{
		std::cerr << "listen";
		return;
	}
	std::cout << "Server listening on port 40713" << std::endl;
	//清空集合，初始化 fd_set
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exset);

	while (1)
	{
		maxfd = listener;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		FD_ZERO(&exset);
		// 告诉 select()：如果有新客户端连接到来（例如浏览器发起连接），请通知我
		FD_SET(listener, &readset);
		for (int i = 0; i < FD_SETSIZE_1024; ++i)
		{
			if (state[i])
			{
				if (i > maxfd)
					maxfd = i;
				// 告诉 select() 监听 FD i 是否“可读”
				// 告诉 select()：如果已有的客户端发来了数据，请通知我
				FD_SET(i, &readset);
				if (state[i]->writing)
					FD_SET(i, &writeset);
			}
		}
		// select() 是阻塞调用，最后一个参数是 nullptr，意味着它会阻塞直到有事件发生，所以不会浪费 CPU。
		if (select(maxfd + 1, &readset, &writeset, &exset, nullptr) < 0)
		{
			std::cerr << "select";
			return;
		}
		// 检查监听 socket listener 是否“就绪”可读
		// true 说明有新的客户端连接等着你用 accept() 接收
		if (FD_ISSET(listener, &readset))
		{
			struct sockaddr_storage ss;
			socklen_t slen = sizeof(ss);
			int fd = accept(listener, (struct sockaddr*)&ss, &slen);
			std::cout << "accept a socket,fd:" << fd;
			if (fd < 0)
				std::cerr << "accept";
			else if (fd > FD_SETSIZE_1024)
			{
				closesocket(fd);
			}
			else {
				make_nonblock(fd);
				//state[fd] = alloc_fd_state();
				state[fd] = alloc_fd_state_unique();
				assert(state[fd]);
			}
		}

		for (int i = 0; i < maxfd + 1; i++)
		{
			int r = 0;
			if (i == listener)
				continue;
			if (FD_ISSET(i, &readset)) {
				r = do_read(i, state[i].get());
			}
			if (r == 0 && FD_ISSET(i, &writeset))
				r = do_write(i, state[i].get());
			if (r)
			{
				std::cerr << "result:" << r << " " << WSAGetLastError();
				//free_fd_state(state[i]);
				state[i] = nullptr;
				closesocket(i);
			}
		}
	}
}

int main()
{

#ifdef _WIN32
	// 存储使用winsock时初始化需要的数据
	WSADATA wsa_data;
	// 调用WSAStartup需要传入Winsock 版本号。
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ret != 0) {
		std::cerr << "WSAStartup failed: " << ret << std::endl;
		return 1;
	}
#endif

	// C语言里的禁用缓冲，能立刻显示printf的消息
	setvbuf(stdout, NULL, _IONBF, 0);
	std::cout << std::unitbuf; // C++无缓冲模式
	run();
	return 0;
}

